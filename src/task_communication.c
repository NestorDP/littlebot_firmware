/* Copyright (c) 2022-2025 Nestor D. Pereira Neto

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "littlebot_firmware/task_communication.h"
 
#define SERIAL_READ_TASK_STACK_SIZE 512
#define SERIAL_READ_TASK_DELAY      100

extern QueueHandle_t g_pCommandVelLeftQueue;
extern QueueHandle_t g_pStatusVelLeftQueue;
extern QueueHandle_t g_pStatusPosLeftQueue;

extern QueueHandle_t g_pCommandVelRightQueue;
extern QueueHandle_t g_pStatusVelRightQueue;
extern QueueHandle_t g_pStatusPosRightQueue;

extern xSemaphoreHandle g_pUartLoggerSemaphore;

extern SerialWrapper console;

static void CommunicationTask(void *pvParameters) {
  (void)pvParameters; /* Unused parameter */

  portTickType ui32WakeTime;
  uint32_t ui32ReadDelay;

  static char rx_msg[100] = "\0";
  static char tx_msg[164] = "\0";
  float command_velocity[2] = {0.0f, 0.0f};
  float status_velocity[2] = {0.0f, 0.0f};
  float status_position[2] = {0.0f, 0.0f};

  ui32ReadDelay = SERIAL_READ_TASK_DELAY;
  ui32WakeTime = xTaskGetTickCount();

  /* Resource to stablish the serial communication */
  SerialWrapper bluetooth;
  SerialWrapperConstructor(&bluetooth, 115200, DATA_STREAM);

  /* Protobuf variables */
  static littlebot_Wheels wheels = littlebot_Wheels_init_zero;
  wheels.side_count = 2;

  /* Initialize Communication Task */
  xSemaphoreTake(g_pUartLoggerSemaphore, portMAX_DELAY);
  console.Printf("Communication task started\n");
  xSemaphoreGive(g_pUartLoggerSemaphore);
  
  /* Wait for motor controllers to start */
  vTaskDelay(200 / portTICK_RATE_MS);
  
  while(1) {
    /* Read wheels status queue */
    xQueueReceive(g_pStatusVelLeftQueue, &status_velocity[LEFT], 0);
    xQueueReceive(g_pStatusPosLeftQueue, &status_position[LEFT], 0);
    xQueueReceive(g_pStatusVelRightQueue, &status_velocity[RIGHT], 0);
    xQueueReceive(g_pStatusPosRightQueue, &status_position[RIGHT], 0);

    /* Load the status into the protobuf message fields */
    wheels.side[LEFT].status_velocity  = status_velocity[LEFT];
    wheels.side[LEFT].status_position  = status_position[LEFT];
    wheels.side[RIGHT].status_velocity = status_velocity[RIGHT];
    wheels.side[RIGHT].status_position = status_position[RIGHT];

    /* Encode and send wheels status message */
    pb_ostream_t stream = pb_ostream_from_buffer((uint8_t *)tx_msg, sizeof(tx_msg));
    pb_encode(&stream, littlebot_Wheels_fields, &wheels);
    bluetooth.Write(tx_msg, stream.bytes_written);

    /* Receive message from serial */
    // bluetooth.Read(rx_msg);

    switch (rx_msg[0]) {
    case 'W':
        // Additional write request - stream is out of scope here, skip for now
        rx_msg[0] = '\0';
      break;
    case 'R':
        // Reset command
        rx_msg[0] = '\0'; 
      break;
    default:
        command_velocity[LEFT] = 0.0f;
        command_velocity[RIGHT] = 0.0f;
    }

    xQueueSend(g_pCommandVelLeftQueue, &command_velocity[LEFT], 0);
    xQueueSend(g_pCommandVelRightQueue, &command_velocity[RIGHT], 0);

    xTaskDelayUntil(&ui32WakeTime, ui32ReadDelay / portTICK_RATE_MS);
  }
}


uint32_t CommunicationTaskInit(void) {
    if( xTaskCreate(CommunicationTask,
                    (const portCHAR *)"Communication Task",
                    SERIAL_READ_TASK_STACK_SIZE,
                    NULL,
                    tskIDLE_PRIORITY + PRIORITY_SERIAL_COMMUNICATION,
                    NULL) != pdTRUE) {
        return(1);
    }

    /* Success. */
    return(0);
}
