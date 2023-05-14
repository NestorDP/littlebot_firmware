// Copyright (c) 2018-2023 Nestor D. Pereira Neto

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

#include "littlebot_drivers/encoder.h"

void EncoderInit(void) {
    EncoderRightConfigure();
    EncoderLeftConfigure();
}


void EncoderRightConfigure(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6);
    GPIOPinConfigure(GPIO_PC5_PHA1);
    GPIOPinConfigure(GPIO_PC6_PHB1);

    QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A | QEI_CONFIG_NO_RESET | QEI_CONFIG_CLOCK_DIR | QEI_CONFIG_NO_SWAP, PPR);
    QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, SysCtlClockGet() / SAMPLE_PERIOD);
    QEIVelocityEnable(QEI1_BASE);

    void (*QEI1IntHandler_ptr)(void) = &EncoderRightHandler;
    QEIIntRegister(QEI1_BASE, *QEI1IntHandler_ptr);
    QEIIntEnable(QEI1_BASE, QEI_INTTIMER);
    QEIEnable(QEI1_BASE);

    QEIPositionSet(QEI1_BASE, 0);
}


void EncoderLeftConfigure(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PD6_PHA0);
    GPIOPinConfigure(GPIO_PD7_PHB0);

    QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A | QEI_CONFIG_NO_RESET | QEI_CONFIG_CLOCK_DIR | QEI_CONFIG_NO_SWAP, PPR);
    QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet() / SAMPLE_PERIOD);
    QEIVelocityEnable(QEI0_BASE);

    void (*QEI0IntHandler_ptr)(void) = &EncoderLeftHandler;
    QEIIntRegister(QEI0_BASE, *QEI0IntHandler_ptr);
    QEIIntEnable(QEI0_BASE, QEI_INTTIMER);
    QEIEnable(QEI0_BASE);

    QEIPositionSet(QEI0_BASE, 0);
}


void EncoderRightHandler(void) {
    QEIIntClear(QEI1_BASE, QEIIntStatus(QEI1_BASE, true));
    ui32EncoderVelRight = QEIVelocityGet(QEI1_BASE);
    ui32EncoderPosRight = QEIPositionGet(QEI1_BASE);
}


void EncoderLeftHandler(void) {
    QEIIntClear(QEI0_BASE, QEIIntStatus(QEI0_BASE, true));
    ui32EncoderVelLeft = QEIVelocityGet(QEI0_BASE);
    ui32EncoderPosLeft = QEIPositionGet(QEI0_BASE);
}


uint32_t EncoderVelocityLeftValue (void) {
    return ui32EncoderVelLeft;
}

uint32_t EncoderVelocityRightValue (void) {
    return ui32EncoderVelRight;
}

uint32_t EncoderPositionLeftValue (void) {
    return ui32EncoderPosLeft;
}

uint32_t EncoderPositionRightValue (void) {
    return ui32EncoderPosRight;
}