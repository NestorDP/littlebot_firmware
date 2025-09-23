// Copyright (c) 2022

#include "littlebot_firmware/task_communication.h"

#define SERIAL_READ_TASK_STACK_SIZE 128         // Stack size in words
#define SERIAL_READ_TASK_DELAY      100
#define RIGHT                       1
#define LEFT                        0

// Command
extern xQueueHandle g_pVelocityLeftQueue;
extern xQueueHandle g_pVelocityRightQueue;

// Status
extern xQueueHandle g_pFBVelocityLeftQueue;
extern xQueueHandle g_pFBVelocityRightQueue;
extern xQueueHandle g_pFBPositionLeftQueue;
extern xQueueHandle g_pFBPositionRightQueue;


static void CommunicationTask(void *pvParameters) {
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
  SerialWrapperConstructor(&bluetooth, 115200);

  // Test nanopb
  //========================================================================
  bluetooth.Write(&bluetooth, "LittleBot Firmware");

  littlebot_Wheels wheels = littlebot_Wheels_init_zero;
  wheels.side_count = 2;
  wheels.side[RIGHT].command_velocity = 1.0;
  wheels.side[RIGHT].status_velocity = 2.0;
  wheels.side[RIGHT].status_position = 2.0;
  wheels.side[LEFT].command_velocity = 3.0;
  wheels.side[LEFT].status_velocity = 4.0;
  wheels.side[LEFT].status_position = 4.0;

  pb_ostream_t stream = pb_ostream_from_buffer((uint8_t *)tx_msg, sizeof(tx_msg));
  if (!pb_encode(&stream, littlebot_Wheels_fields, &wheels)) {
      bluetooth.Write(&bluetooth, "Encoding failed");
  } else {
    static char hex_buffer[256];  // 2 chars per byte + null terminator
    for(int i = 0; i < stream.bytes_written; i++) {
        sprintf(&hex_buffer[i*2], "%02X", tx_msg[i]);
    }
    bluetooth.Write(&bluetooth, hex_buffer);
  }

  bluetooth.Write(&bluetooth, "ooooooooooooooooooooo");
  //========================================================================

 
  while(1) {
    /* Read velocity status queue */
    xQueueReceive(g_pFBVelocityLeftQueue, &feed_back_velocity[0], 0);
    xQueueReceive(g_pFBVelocityRightQueue, &feed_back_velocity[1], 0);

    /* Read position status queue */
    xQueueReceive(g_pFBPositionLeftQueue, &feed_back_position[0], 0);
    xQueueReceive(g_pFBPositionRightQueue, &feed_back_position[1], 0);

    /* Receive message from serial */
    bluetooth.Read(&bluetooth, rx_msg);

    switch (rx_msg[0]) {
      case 'W':
        bluetooth.Write(&bluetooth, "1");
        rx_msg[0] = '\0';
      break;
      case 'R':
        bluetooth.Write(&bluetooth, tx_msg);
        rx_msg[0] = '\0'; 
      break;
      default:
        velocity[0] = 0.0;
        velocity[1] = 0.0;
    }

    xQueueSend(g_pVelocityLeftQueue, &velocity[0], 0);
    xQueueSend(g_pVelocityRightQueue, &velocity[1], 0);

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
