// Copyright (c) 2022

#include "littlebot_firmware/task_serial_read.h"

#define SERIAL_READ_TASK_STACK_SIZE 128         // Stack size in words
#define SERIAL_READ_TASK_DELAY      100


extern Serialization protocol;
extern SerialInterface bluetooth;

extern xQueueHandle g_pVelocityQueue;
extern xSemaphoreHandle g_pSerializationSemaphore;


static void SerialReadTask(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32ReadDelay;

    char protocol_msg[80] = "\0";
    float velocity[2];

    ui32ReadDelay = SERIAL_READ_TASK_DELAY;
    ui32WakeTime = xTaskGetTickCount();

    while(1) {
        xSemaphoreTake(g_pSerializationSemaphore, portMAX_DELAY);
        bluetooth.Read(&bluetooth, protocol_msg);
        xSemaphoreGive(g_pSerializationSemaphore);

        protocol.Decode(&protocol, protocol_msg, &velocity[0], &velocity[1]);
        
        xQueueSend(g_pVelocityQueue, &velocity, 0);

        xTaskDelayUntil(&ui32WakeTime, ui32ReadDelay / portTICK_RATE_MS);
    }
}


uint32_t SerialReadTaskInit(void *param) {
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
