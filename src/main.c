/* Copyright (c) 2022-2025 Nestor D. Pereira Neto

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "littlebot_drivers/motor.h" /* To use motor macros */
#include "littlebot_firmware/serial_wrapper.h"
#include "littlebot_firmware/task_motor_controller.h"
#include "littlebot_firmware/task_communication.h"
#include "littlebot_firmware/priorities.h"

/* The mutex that protects concurrent access of UART from multiple tasks. */
xSemaphoreHandle g_pUartLoggerSemaphore;

/* The queue that holds variables to sharade between tasks. */
xQueueHandle g_pCommandVelLeftQueue;
xQueueHandle g_pCommandVelRightQueue;
xQueueHandle g_pStatusVelLeftQueue;
xQueueHandle g_pStatusVelRightQueue;
xQueueHandle g_pStatusPosRightQueue;
xQueueHandle g_pStatusPosLeftQueue;

/* The error routine that is called if the driver library encounters an error. */
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line){}
#endif

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    /* This function can not return, so loop forever.  Interrupts are disabled
     * on entry to this function, so no processor interrupts will interrupt
     * this loop.
     */
    (void)xTask;
    (void)pcTaskName;
    while(1) {}
}


int main(void) {
    /* Set the clocking to run at 80 MHz from the PLL. */
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    IntMasterEnable();

    /* Create queues for exchange variables between tasks. */
    g_pCommandVelLeftQueue  = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pStatusVelLeftQueue   = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pStatusPosLeftQueue   = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);

    g_pCommandVelRightQueue = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pStatusVelRightQueue  = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pStatusPosRightQueue  = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);

    /* Create semaphore to protect the serial port used in logging. */
    g_pUartLoggerSemaphore = xSemaphoreCreateMutex();

    SerialWrapper debug_console;
    SerialWrapperConstructor(&debug_console, 115200, DEBUG_CONSOLE);
    debug_console.Printf("LittleBot starting...\n");


    /* Create the COMMUNICATION task. */
    if(CommunicationTaskInit() != 0) {
        while(1) {}
    }
   
    /* Create the LEFT MOTOR CONTROLLER task. */
    if(MotorControllerTaskInit(LEFT, "Motor left") != 0) {
        while(1) {}
    }

    /* Create the RIGHT MOTOR CONTROLLER task. */
    if(MotorControllerTaskInit(RIGHT, "Motor right") != 0) {
        while(1) {}
    }

    /* Start the scheduler.  This should not return. */
    vTaskStartScheduler();

    /* In case the scheduler returns for some reason, print an error and loop
     * forever.
     */

    while(1) {}
}
