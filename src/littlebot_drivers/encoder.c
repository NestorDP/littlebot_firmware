/*
 * encoder.c
 *
 *  Created on: 22 de abr de 2018
 *      Author: nestor
 */

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
}


void EncoderRightHandler(void) {
    QEIIntClear(QEI1_BASE, QEIIntStatus(QEI1_BASE, true));
    ui32EncoderVelRight = QEIVelocityGet(QEI1_BASE);
}


void EncoderLeftHandler(void) {
    QEIIntClear(QEI0_BASE, QEIIntStatus(QEI0_BASE, true));
    ui32EncoderVelLeft = QEIVelocityGet(QEI0_BASE);
}


uint32_t EncoderLeftValue (void) {
    return ui32EncoderVelLeft;
}

uint32_t EncoderRightValue (void) {
    return ui32EncoderVelRight;
}