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
#include "littlebot_firmware/types.h"
 
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

  /* Initialize wheels data */
  WheelsData_t wheels_data = {
    .left = {
      .command_velocity = 1.0f,
      .status_velocity = 0.0f,
      .status_position = 0.0f
    },
    .right = {
      .command_velocity = 2.0f,
      .status_velocity = 1.0f,
      .status_position = 0.0f
    }
  };
  
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
    xQueueReceive(g_pStatusVelLeftQueue, &wheels_data.left.status_velocity, 0);
    xQueueReceive(g_pStatusPosLeftQueue, &wheels_data.left.status_position, 0);
    xQueueReceive(g_pStatusVelRightQueue, &wheels_data.right.status_velocity, 0);
    xQueueReceive(g_pStatusPosRightQueue, &wheels_data.right.status_position, 0);

    /* Load the status into the protobuf message fields */
    wheels.side[LEFT].command_velocity = wheels_data.left.command_velocity;
    wheels.side[LEFT].status_velocity = wheels_data.left.status_velocity;
    wheels.side[LEFT].status_position = wheels_data.left.status_position;
    wheels.side[RIGHT].command_velocity = wheels_data.right.command_velocity;
    wheels.side[RIGHT].status_velocity = wheels_data.right.status_velocity;
    wheels.side[RIGHT].status_position = wheels_data.right.status_position;

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
        // wheels_data.left.command_velocity = 0.0f;
        // wheels_data.right.command_velocity = 0.0f;
    }

    xQueueSend(g_pCommandVelLeftQueue, &wheels_data.left.command_velocity, 0);
    xQueueSend(g_pCommandVelRightQueue, &wheels_data.right.command_velocity, 0);

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
