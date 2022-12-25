// Copyright (c) 2022

#include "littlebot_firmware/task_serial_write.h"

#define SERIAL_WRITE_TASK_STACK_SIZE 128         // Stack size in words
#define SERIAL_READ_TOGGLE_DELAY     100


// xQueueHandle g_pLittlebotQueue;

extern SerialInterface s;
extern Serialization comm;

extern xSemaphoreHandle g_pSerializationSemaphore;

static void SerialWriteTask(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32ToggleDelay;

    ui32ToggleDelay = SERIAL_READ_TOGGLE_DELAY;
    ui32WakeTime = xTaskGetTickCount();

    // SerialInterface *s = (SerialInterface *) pvParameters;


    while(1) {
        xSemaphoreTake(g_pSerializationSemaphore, portMAX_DELAY);
        // s.Write(&s, "DENTRO DA TASK");
        comm.SendMessage(&comm);
        xSemaphoreGive(g_pSerializationSemaphore);

        xTaskDelayUntil(&ui32WakeTime, ui32ToggleDelay / portTICK_RATE_MS);
    }
}


uint32_t SerialWriteTaskInit(void *param) {

    if( xTaskCreate(SerialWriteTask,
                   (const portCHAR *)"SERIAL_WRITE",
                   SERIAL_WRITE_TASK_STACK_SIZE,
                   param,
                   tskIDLE_PRIORITY + PRIORITY_SERIAL_WRITE_TASK,
                   NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
