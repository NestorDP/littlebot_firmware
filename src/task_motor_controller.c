// Copyright (c) 2022


#include "littlebot_firmware/task_motor_controller.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128         // Stack size in words
#define MOTOR_CONTROLLER_TASK_DELAY      100

extern xQueueHandle g_pVelocityQueue;
extern xQueueHandle g_pFBVelocityQueue;

uint8_t side_m;

static void vTaskMotorController(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32MotorTaskDelay;   
    float feed_back_velocity[2];
    uint8_t *side_motor;
    
    side_motor = (uint8_t*)pvParameters;

    MotorInterface motor;
    
    MotorInterfaceConstruct(&motor, *side_motor);
    ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
    ui32WakeTime = xTaskGetTickCount();

    feed_back_velocity[0] = 18.152;
    feed_back_velocity[1] = 29.262;
    
    while(1) {
//         if( xQueueReceive(g_pLEDQueue, &i8Message, 0) == pdPASS ) {
//             if(i8Message == LEFT_BUTTON) {
//                 g_pui32Colors[g_ui8ColorsIndx] = 0x0000;
//                 g_ui8ColorsIndx++;
//                 if(g_ui8ColorsIndx > 2) {
//                     g_ui8ColorsIndx = 0;
//                 }
//                 g_pui32Colors[g_ui8ColorsIndx] = 0x8000;
//                 RGBColorSet(g_pui32Colors);
//                 xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
//                 xSemaphoreGive(g_pUARTSemaphore);
//             }

//             if(i8Message == RIGHT_BUTTON) {
//                 ui32LEDToggleDelay *= 2;
//                 if(ui32LEDToggleDelay > 1000) {
//                     ui32LEDToggleDelay = SERIAL_READ_TOGGLE_DELAY / 2;
//                 }
//                 xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
//                 xSemaphoreGive(g_pUARTSemaphore);
//             }
//         }
    feed_back_velocity[motor.GetSide(&motor)];
    if(xQueueSend(g_pFBVelocityQueue, &feed_back_velocity, portMAX_DELAY) != pdPASS) {
        // Error. The queue should never be full. If so print the
        // error message on UART and wait for ever.
        while(1) {}
    }

    xTaskDelayUntil(&ui32WakeTime, ui32MotorTaskDelay / portTICK_RATE_MS);
    }
}


uint32_t MotorControllerTaskInit(uint8_t side, const char *name, UBaseType_t priority) {
    side_m = side;
    if( xTaskCreate(vTaskMotorController, 
                    (const portCHAR *)name, 
                    MOTOR_CONTROLLER_TASK_STACK_SIZE, 
                    (void *) &side_m,
                    tskIDLE_PRIORITY + priority, 
                    NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}



