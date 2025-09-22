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

#include "pb_encode.h"
#include "pb_decode.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "littlebot_firmware/led_task.h"
#include "littlebot_firmware/motor_interface.h"
#include "littlebot_firmware/serial_interface.h"
#include "littlebot_firmware/switch_task.h"
#include "littlebot_firmware/task_motor_controller.h"
#include "littlebot_firmware/task_communication.h"

#include "littlebot_firmware/priorities.h"
#include "littlebot_msg.pb.h"

/* The mutex that protects concurrent access of UART from multiple tasks. */
xSemaphoreHandle g_pSerializationSemaphore;

/* The queue that holds variables to sharade between tasks. */
xQueueHandle g_pLEDQueue;
xQueueHandle g_pVelocityLeftQueue;
xQueueHandle g_pVelocityRightQueue;
xQueueHandle g_pFBVelocityLeftQueue;
xQueueHandle g_pFBVelocityRightQueue;
xQueueHandle g_pFBPositionRightQueue;
xQueueHandle g_pFBPositionLeftQueue;

/* Resource to stablish the serial communication */
SerialInterface bluetooth;

/* The error routine that is called if the driver library encounters an error. */
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line){}
#endif

void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName) {
    /* This function can not return, so loop forever.  Interrupts are disabled
     * on entry to this function, so no processor interrupts will interrupt
     * this loop.
     */
    (void)pxTask;
    (void)pcTaskName;
    while(1) {}
}


int main(void) {
    /* Set the clocking to run at 80 MHz from the PLL. */
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    IntMasterEnable();

    /* Create queues for exchange variables between tasks. */
    g_pVelocityLeftQueue    = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pVelocityRightQueue   = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pFBVelocityLeftQueue  = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pFBVelocityRightQueue = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pFBPositionLeftQueue  = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pFBPositionRightQueue = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);

    /* Create semaphore to protect the serial port. */
    g_pSerializationSemaphore = xSemaphoreCreateMutex();

    /* Create communication object */
    SerialInterfaceConstructor(&bluetooth, 115200);

    bluetooth.Write(&bluetooth, "LittleBot Firmware");

    /* Create the COMMUNICATION task. */
    // if(CommunicationTaskInit() != 0) {
    //     while(1) {}
    // }
   
    /* Create the LEFT MOTOR CONTROLLER task. */
    // if(MotorControllerTaskInit(1, "Motor left") != 0) {
    //     while(1) {}
    // }

    /* Create the RIGHT MOTOR CONTROLLER task. */
    // if(MotorControllerTaskInit(0, "Motor right") != 0) {
    //     while(1) {}
    // }

    /* Start the scheduler.  This should not return. */
    vTaskStartScheduler();

    /* In case the scheduler returns for some reason, print an error and loop
     * forever.
     */

    while(1) {}
}
