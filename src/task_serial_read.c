// Copyright (c) 2022

#include "littlebot_firmware/task_serial_read.h"

#define SERIAL_READ_TASK_STACK_SIZE 128         // Stack size in words
#define SERIAL_READ_TASK_DELAY      100


extern Serialization protocol;
extern SerialInterface bluetooth;

// Command
extern xQueueHandle g_pVelocityLeftQueue;
extern xQueueHandle g_pVelocityRightQueue;

// Status
extern xQueueHandle g_pFBVelocityLeftQueue;
extern xQueueHandle g_pFBVelocityRightQueue;
extern xQueueHandle g_pFBPositionLeftQueue;
extern xQueueHandle g_pFBPositionRightQueue;


static void SerialReadTask(void *pvParameters) {
  portTickType ui32WakeTime;
  uint32_t ui32ReadDelay;

  char rx_msg[80] = "\0";
  char tx_msg[80] = "\0";
  float velocity[2];
  float feed_back_velocity[2];
  float feed_back_position[2];

  ui32ReadDelay = SERIAL_READ_TASK_DELAY;
  ui32WakeTime = xTaskGetTickCount();

  uint32_t num = 0;

  while(1) {
    // Read velocity status queue 
    xQueueReceive(g_pFBVelocityRightQueue, &feed_back_velocity[0], 0);
    xQueueReceive(g_pFBVelocityLeftQueue, &feed_back_velocity[1], 0);

    // Read velocity status queue 
    xQueueReceive(g_pFBPositionRightQueue, &feed_back_position[0], 0);
    xQueueReceive(g_pFBPositionLeftQueue, &feed_back_position[1], 0);

    // Receive message from serial
    num = bluetooth.Read(&bluetooth, rx_msg);

    switch (rx_msg[0]) {
      case 'W':
        protocol.Decode(&protocol, &rx_msg[1], &velocity[0], &velocity[1]);
        bluetooth.Write(&bluetooth, "1");
        rx_msg[0] = '\0';
      break;
      case 'R':
        protocol.Encode(&protocol, tx_msg, 
                &feed_back_velocity[0], &feed_back_velocity[1],
                &feed_back_position[0], &feed_back_position[1]);
        bluetooth.Write(&bluetooth, tx_msg);
        rx_msg[0] = '\0'; 
      break;
      default:
        // velocity[0] = 0.0;
        // velocity[1] = 0.0;
    }

    xQueueSend(g_pVelocityRightQueue, &velocity[0], 0);
    xQueueSend(g_pVelocityLeftQueue, &velocity[1], 0);

    xTaskDelayUntil(&ui32WakeTime, ui32ReadDelay / portTICK_RATE_MS);
  }
}


uint32_t SerialReadTaskInit(void) {
    if( xTaskCreate(SerialReadTask,
                    (const portCHAR *)"Serial read",
                    SERIAL_READ_TASK_STACK_SIZE,
                    NULL,
                    tskIDLE_PRIORITY + PRIORITY_SERIAL_READ_TASK,
                    NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
