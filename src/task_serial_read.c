// // Copyright (c) 2022

// #include <stdbool.h>
// #include <stdint.h>

// #include "inc/hw_memmap.h"
// #include "inc/hw_types.h"
// #include "driverlib/gpio.h"
// #include "driverlib/rom.h"
// #include "utils/uartstdio.h"

// #include "FreeRTOS.h"
// #include "task.h"
// #include "queue.h"
// #include "semphr.h"

// #include "littlebot_firmware/serial_read_task.h"

// #define SERIAL_READ_ITEM_SIZE    sizeof(uint8_t)
// #define SERIAL_READ_QUEUE_SIZE   5
// #define SERIAL_READ_TOGGLE_DELAY 100

// xQueueHandle g_pLittlebotQueue;

// extern xSemaphoreHandle g_pSerializationSemaphore;

// static void LEDTask(void *pvParameters)
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


// uint32_t LEDTaskInit(void)
// {
//     //
//     // Initialize the GPIOs and Timers that drive the three LEDs.
//     //
//     RGBInit(1);
//     RGBIntensitySet(0.1f);

//     //
//     // Turn on the Green LED
//     //
//     g_ui8ColorsIndx = 2;
//     g_pui32Colors[g_ui8ColorsIndx] = 0x8000;
//     RGBColorSet(g_pui32Colors);

//     //
//     // Print the current loggling LED and frequency.
//     //
//     //UARTprintf("\nLed %d is blinking. [R, G, B]\n", g_ui8ColorsIndx);
//     //UARTprintf("Led blinking frequency is %d ms.\n", (SERIAL_READ_TOGGLE_DELAY * 2));

//     //
//     // Create a queue for sending messages to the LED task.
//     //
//     g_pLEDQueue = xQueueCreate(SERIAL_READ_QUEUE_SIZE, SERIAL_READ_ITEM_SIZE);

//     //
//     // Create the LED task.
//     //
//     if( xTaskCreate(SerialReadTask, (const portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
//                    tskIDLE_PRIORITY + PRIORITY_SERIAL_READ_TASK, NULL) != pdTRUE) {
//         return(1);
//     }

//     //
//     // Success.
//     //
//     return(0);
// }





// // recebe ponteiro para objeto serialization
// // inicia task
// //