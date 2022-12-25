// Copyright (c) 2022

#include "littlebot_firmware/task_serial_write.h"

#define SERIAL_WRITE_TASK_STACK_SIZE 128         // Stack size in words
#define SERIAL_READ_TOGGLE_DELAY     100


// xQueueHandle g_pLittlebotQueue;

extern Serialization protocol;
extern SerialInterface bluetooth;
extern xSemaphoreHandle g_pSerializationSemaphore;

static void SerialWriteTask(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32ToggleDelay;
    char protocol_msg[200];

    ui32ToggleDelay = SERIAL_READ_TOGGLE_DELAY;
    ui32WakeTime = xTaskGetTickCount();
    float a = 47.123;
    float b = 53.123;

    float val;
    val = atof(protocol_msg);

    while(1) {
        xSemaphoreTake(g_pSerializationSemaphore, portMAX_DELAY);
        protocol.Encode(&protocol, protocol_msg, &a, &b);
        bluetooth.Write(&bluetooth, protocol_msg);
        xSemaphoreGive(g_pSerializationSemaphore);

        xTaskDelayUntil(&ui32WakeTime, ui32ToggleDelay / portTICK_RATE_MS);
    }
}


uint32_t SerialWriteTaskInit(void) {

    if( xTaskCreate(SerialWriteTask,
                   (const portCHAR *)"SERIAL_WRITE",
                   SERIAL_WRITE_TASK_STACK_SIZE,
                   NULL,
                   tskIDLE_PRIORITY + PRIORITY_SERIAL_WRITE_TASK,
                   NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
