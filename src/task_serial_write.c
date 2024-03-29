// Copyright (c) 2022

#include "littlebot_firmware/task_serial_write.h"

#define SERIAL_WRITE_TASK_STACK_SIZE 128 // Stack size in words
#define SERIAL_WRITE_TASK_DELAY 30

extern Serialization protocol;
extern SerialInterface bluetooth;

extern xQueueHandle g_pFBVelocityLeftQueue;
extern xQueueHandle g_pFBVelocityRightQueue;
extern xQueueHandle g_pFBPositionLeftQueue;
extern xQueueHandle g_pFBPositionRightQueue;

extern xSemaphoreHandle g_pSerializationSemaphore;

static void SerialWriteTask(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32WriteDelay;

    char protocol_msg[100];
    float feed_back_velocity[2];
    float feed_back_position[2];

    ui32WriteDelay = SERIAL_WRITE_TASK_DELAY;
    ui32WakeTime = xTaskGetTickCount();

    float val;
    // I still don't know why when I remove this line the pow() function is not found by
    // the linker
    val = atof(protocol_msg);

    while (1){
        xQueueReceive(g_pFBVelocityRightQueue, &feed_back_velocity[0], 0);
        xQueueReceive(g_pFBVelocityLeftQueue, &feed_back_velocity[1], 0);

        xQueueReceive(g_pFBPositionRightQueue, &feed_back_position[0], 0);
        xQueueReceive(g_pFBPositionLeftQueue, &feed_back_position[1], 0);

        protocol.Encode(&protocol, protocol_msg, 
                        &feed_back_velocity[0], &feed_back_velocity[1],
                        &feed_back_position[0], &feed_back_position[1]);

        xSemaphoreTake(g_pSerializationSemaphore, portMAX_DELAY);
        bluetooth.Write(&bluetooth, protocol_msg);
        xSemaphoreGive(g_pSerializationSemaphore);

        xTaskDelayUntil(&ui32WakeTime, ui32WriteDelay / portTICK_RATE_MS);
    }
}

uint32_t SerialWriteTaskInit(void) {
    if (xTaskCreate(SerialWriteTask,
                    (const portCHAR *)"Serial writ",
                    SERIAL_WRITE_TASK_STACK_SIZE,
                    NULL,
                    tskIDLE_PRIORITY + PRIORITY_SERIAL_WRITE_TASK,
                    NULL) != pdTRUE) {
        return (1);
    }

    // Success.
    return (0);
}
