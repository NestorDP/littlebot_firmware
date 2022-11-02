#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "task.h"

static void prvLedToggleTask();

/* Priorities at which the tasks are created. */
#define mainTOGGLE_LED_PRIORITY         ( tskIDLE_PRIORITY + 1 )
#define TOGGLE_LED_DELAY_MS             200

void main_blinky()
{
    /* create task */
    xTaskCreate( prvLedToggleTask, "LED", configMINIMAL_STACK_SIZE, NULL, mainTOGGLE_LED_PRIORITY, NULL );

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

}

static void prvLedToggleTask( void *pvParameters )
{
    // Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
        //
        // Turn on the LED.
        //
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);

        // Wait a bit
        vTaskDelay( TOGGLE_LED_DELAY_MS);

        //
        // Turn off the LED.
        //
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);

        // Wait a bit
        vTaskDelay( TOGGLE_LED_DELAY_MS );
    }
}

uint32_t g_ui32SysClock;

void prvSetupHardware()
{
    // Configure PLL
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN |
                                         SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    //
    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
}
