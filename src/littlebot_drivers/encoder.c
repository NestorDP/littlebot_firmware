/*
 * encoder.c
 *
 *  Created on: 22 de abr de 2018
 *      Author: nestor
 */

#include "littlebot_drivers/encoder.h"


void EncoderInit(void){
	EncoderRightConfigure();
}


void EncoderRightConfigure(void){
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


// void EncoderGpioLeft(void){
	
// }


// uint32_t EncoderGetLeftValue(void){
	
// }	


void EncoderRightHandler(){
	QEIIntClear(QEI1_BASE, QEIIntStatus(QEI1_BASE, true));

	flag = ~flag;
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, flag & GPIO_PIN_3);

    ui32EncoderVelRight = QEIVelocityGet(QEI1_BASE);			
}


// void EncoderLeftHandler(){

	
// }
