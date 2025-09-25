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
 
#define SERIAL_READ_TASK_STACK_SIZE 128
#define SERIAL_READ_TASK_DELAY      100

extern xQueueHandle g_pCommandVelLeftQueue;
extern xQueueHandle g_pStatusVelLeftQueue;
extern xQueueHandle g_pStatusPosLeftQueue;

extern xQueueHandle g_pCommandVelRightQueue;
extern xQueueHandle g_pStatusVelRightQueue;
extern xQueueHandle g_pStatusPosRightQueue;


static void CommunicationTask(void *pvParameters) {
  (void)pvParameters; /* Unused parameter */

  portTickType ui32WakeTime;
  uint32_t ui32ReadDelay;

  static char rx_msg[100] = "\0";
  static char tx_msg[100] = "\0";
  float velocity[2] = {0.0, 0.0};
  float feed_back_velocity[2] = {0.0, 0.0};
  float feed_back_position[2] = {0.0, 0.0};

  ui32ReadDelay = SERIAL_READ_TASK_DELAY;
  ui32WakeTime = xTaskGetTickCount();

  /* Resource to stablish the serial communication */
  SerialWrapper bluetooth;
  SerialWrapperConstructor(&bluetooth, 115200, DATA_STREAM);

  littlebot_Wheels wheels = littlebot_Wheels_init_zero;
  wheels.side_count = 2;
  wheels.side[LEFT].command_velocity = 1.5f;
  wheels.side[LEFT].status_velocity = 1.45f;
  wheels.side[LEFT].status_position = 45.2f;
  wheels.side[RIGHT].command_velocity = 1.5f;
  wheels.side[RIGHT].status_velocity = 1.48f;
  wheels.side[RIGHT].status_position = 46.8f;

  pb_ostream_t stream = pb_ostream_from_buffer((uint8_t *)tx_msg, sizeof(tx_msg));
  if (!pb_encode(&stream, littlebot_Wheels_fields, &wheels)) {
      // bluetooth.Write("Encoding failed");
  } else {
    bluetooth.Write(tx_msg, stream.bytes_written);
  }

 
  while(1) {
    /* Read velocity status queue */
    xQueueReceive(g_pStatusVelLeftQueue, &feed_back_velocity[0], 0);
    xQueueReceive(g_pStatusVelRightQueue, &feed_back_velocity[1], 0);

    /* Read position status queue */
    xQueueReceive(g_pStatusPosLeftQueue, &feed_back_position[0], 0);
    xQueueReceive(g_pStatusPosRightQueue, &feed_back_position[1], 0);

    /* Receive message from serial */
    // bluetooth.Read(rx_msg);

    switch (rx_msg[0]) {
      case 'W':
        // bluetooth.Write("1");
        rx_msg[0] = '\0';
      break;
      case 'R':
        // bluetooth.Write(tx_msg);
        rx_msg[0] = '\0'; 
      break;
      default:
        velocity[0] = 0.0;
        velocity[1] = 0.0;
    }

    xQueueSend(g_pCommandVelLeftQueue, &velocity[0], 0);
    xQueueSend(g_pCommandVelRightQueue, &velocity[1], 0);

    xTaskDelayUntil(&ui32WakeTime, ui32ReadDelay / portTICK_RATE_MS);
  }
}


uint32_t CommunicationTaskInit(void) {
    if( xTaskCreate(CommunicationTask,
                    (const portCHAR *)"Serial read",
                    SERIAL_READ_TASK_STACK_SIZE,
                    NULL,
                    tskIDLE_PRIORITY + PRIORITY_SERIAL_COMMUNICATION,
                    NULL) != pdTRUE) {
        return(1);
    }

    /* Success. */
    return(0);
}
