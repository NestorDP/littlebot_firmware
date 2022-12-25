// Copyright (c) 2022 Nestor D. Pereira Neto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "littlebot_firmware/led_task.h"
#include "littlebot_firmware/switch_task.h"
#include "littlebot_firmware/task_serial_write.h"

#include "littlebot_firmware/motor_interface.h"
#include "littlebot_firmware/serial.h"
#include "littlebot_firmware/serialization.h"

// The item size and queue size for the LED message queue.
#define LED_ITEM_SIZE           sizeof(uint8_t)
#define LED_QUEUE_SIZE          5

#define VELOCITY_ITEM_SIZE    sizeof(float)
#define VELOCITY_QUEUE_SIZE   8


// The mutex that protects concurrent access of UART from multiple tasks.
xSemaphoreHandle g_pSerializationSemaphore;

// The queue that holds variables to sharade between tasks.
xQueueHandle g_pLEDQueue;
xQueueHandle g_pVelocityQueue;
xQueueHandle g_pFBVelocityQueue;


SerialInterface s;
Serialization comm;


// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line){}
#endif



void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName) {
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    while(1) {}
}


int main(void) {
    // Set the clocking to run at 50 MHz from the PLL.
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    
    // Create communication object
    // SerialInterface s;
    // SerialInterfaceContruct(&s, 115200);
    SerializationConstruct(&comm);


    // Create motors objects
    // MotorInterface motor_left, motor_right;
    // MotorInterfaceConstruct(&motor_left, left);
    // MotorInterfaceConstruct(&motor_right, right);

    // Create queues for exchange variables between tasks.
    g_pVelocityQueue   = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pFBVelocityQueue = xQueueCreate(VELOCITY_QUEUE_SIZE, VELOCITY_ITEM_SIZE);
    g_pLEDQueue        = xQueueCreate(LED_QUEUE_SIZE, LED_ITEM_SIZE);

    // Create semaphore to protect the serial port.
    g_pSerializationSemaphore = xSemaphoreCreateMutex();

    // Create the SERIAL_WRITE task.
    if(SerialWriteTaskInit((void *) &s) != 0) {
        while(1) {}
    }

    // Create the SERIAL_READ task.
    // if(SerialReadTaskInit((void *) &s) != 0) {
    //     while(1) {}
    // }
   
    // Create the MOTOR CONTROLLER task.
    // if(MotorControllerTaskInit(&motor_left) != 0) {
    //     while(1) {}
    // }

     
    // Create the LED task.
    if(LEDTaskInit() != 0) {
        while(1) {}
    }

    // Create the SWITCH task.
    if(SwitchTaskInit() != 0) {
        while(1) {}
    }

    // Start the scheduler.  This should not return.
    vTaskStartScheduler();

    // In case the scheduler returns for some reason, print an error and loop
    // forever.

    while(1) {}
}
