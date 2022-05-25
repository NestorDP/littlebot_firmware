#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "stdlib.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#include "utils/uartstdio.h"
#include "utils/uartstdio.c"

#include "app/motor.h"
#include "app/bluetooth.h"
#include "app/encoder.h"


int main(void)
{
    volatile uint32_t ui32Loop;
    uint16_t A = 0;
    uint16_t B = 0;

    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    
    MotorConfigure();
    BluetoothConfigure();
    // EncoderConfigure();

    while(1)
    {
        BluetoothRead(&A, &B);
        MotorVelocity(RIGHT, A, FORWARD);
        MotorVelocity(LEFT, B, FORWARD);
        // UARTprintf("\nola MAIN!!!%d right  %d left\n", pulse_right, pulse_left);
        UARTprintf("\nola MAIN!!!%d right  %d left\n", A, B);
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }
}
















/*
*/