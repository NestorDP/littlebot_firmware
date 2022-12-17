// Copyright (c) 2022

#include "littlebot_firmware/task_serial_read.h"

#define SERIAL_READ_TASK_STACK_SIZE 128         // Stack size in words

#define SERIAL_READ_TOGGLE_DELAY     100


// xQueueHandle g_pLittlebotQueue;

// extern xSemaphoreHandle g_pSerializationSemaphore;

static void SerialReadTask(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32ToggleDelay;
    char msg[10];
    Serialization *comm = (Serialization *) pvParameters;

    ui32ToggleDelay = SERIAL_READ_TOGGLE_DELAY;
    ui32WakeTime = xTaskGetTickCount();

    SerialInterface *s = (SerialInterface *) pvParameters;

    while(1) {
        // comm->SetVelocity(comm)
        // comm->SendMessage(comm);

        s->Read(s, msg, 10);
        xTaskDelayUntil(&ui32WakeTime, ui32ToggleDelay / portTICK_RATE_MS);
        RGBDisable();
        xTaskDelayUntil(&ui32WakeTime, ui32ToggleDelay / portTICK_RATE_MS);
    }
}


uint32_t SerialReadTaskInit(void *param) {
    // Create the serial write task.
    SerialInterface *ser;
    ser = (SerialInterface *) param;

    ser->Write(ser, "teste");

    if( xTaskCreate(SerialReadTask,
                   (const portCHAR *)"SERIAL_READ",
                   SERIAL_READ_TASK_STACK_SIZE,
                   (void *) ser,
                   tskIDLE_PRIORITY + PRIORITY_SERIAL_READ_TASK,
                   NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
