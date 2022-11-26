// Copyright (c) 2022


#include "littlebot_firmware/motor_controller_task.h"

// extern xQueueHandle g_pVelocity;
// extern xQueueHandle g_pFeedBackVelocity;


static void vTaskMotorController(void *pvParameters)
// {
//     portTickType ui32WakeTime;
//     uint32_t ui32LEDToggleDelay;
//     uint8_t i8Message;

//     ui32LEDToggleDelay = SERIAL_READ_TOGGLE_DELAY;

//     ui32WakeTime = xTaskGetTickCount();

//     while(1) {
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

//         RGBEnable();
//         xTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);
//         RGBDisable();
//         xTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);
//     }
// }


uint32_t MotorControllerTaskInit(MotorInterface *mt) {
    
    motor = mt;

    // Create the motor controller task.
    if( xTaskCreate(
                vTaskMotorController, 
                "LeftMotorController", 
                LEDTASKSTACKSIZE, 
                NULL,
                tskIDLE_PRIORITY + PRIORITY_SERIAL_READ_TASK, 
                NULL
                ) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
