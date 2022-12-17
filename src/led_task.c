

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "littlebot_drivers/rgb.h"
#include "littlebot_drivers/buttons.h"

#include "littlebot_firmware/led_task.h"
#include "littlebot_firmware/priorities.h"

#include "littlebot_api/serial.h"


#define LEDTASKSTACKSIZE        128         // Stack size in words

#define LED_TOGGLE_DELAY        100

// The queue that holds messages sent to the LED task.
extern xQueueHandle g_pLEDQueue;

// [G, R, B] range is 0 to 0xFFFF per color.
static uint32_t g_pui32Colors[3] = { 0x0000, 0x0000, 0x0000 };
static uint8_t g_ui8ColorsIndx;

extern xSemaphoreHandle g_pUARTSemaphore;

static void LEDTask(void *pvParameters)
{
    portTickType ui32WakeTime;
    uint32_t ui32LEDToggleDelay;
    uint8_t i8Message;

    // Initialize the LED Toggle Delay to default value.
    ui32LEDToggleDelay = LED_TOGGLE_DELAY;

    // Get the current tick count.
    ui32WakeTime = xTaskGetTickCount();

    while(1) {
        // Read the next message, if available on queue.

        if( xQueueReceive(g_pLEDQueue, &i8Message, 0) == pdPASS ) {
            // If left button, update to next LED.
            if(i8Message == LEFT_BUTTON) {
                // Update the LED buffer to turn off the currently working.
                g_pui32Colors[g_ui8ColorsIndx] = 0x0000;

                // Update the index to next LED
                g_ui8ColorsIndx++;
                if(g_ui8ColorsIndx > 2) {
                    g_ui8ColorsIndx = 0;
                }

                // Update the LED buffer to turn on the newly selected LED.
                g_pui32Colors[g_ui8ColorsIndx] = 0x8000;

                // Configure the new LED settings.
                RGBColorSet(g_pui32Colors);
            }

            // If right button, update delay time between toggles of led.
            if(i8Message == RIGHT_BUTTON) {
                ui32LEDToggleDelay *= 2;
                if(ui32LEDToggleDelay > 1000) {
                    ui32LEDToggleDelay = LED_TOGGLE_DELAY / 2;
                }
            }
        }


        // Turn on the LED.
        RGBEnable();

        // Wait for the required amount of time.
        xTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);

        // Turn off the LED.
        RGBDisable();

        // Wait for the required amount of time.
        xTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);
    }
}

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t LEDTaskInit(void)
{
    // Initialize the GPIOs and Timers that drive the three LEDs.
    RGBInit(1);
    RGBIntensitySet(0.1f);

    // Turn on the Green LED
    g_ui8ColorsIndx = 2;
    g_pui32Colors[g_ui8ColorsIndx] = 0x8000;
    RGBColorSet(g_pui32Colors);



    // Create the LED task.
    if( xTaskCreate(LEDTask, (const portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
