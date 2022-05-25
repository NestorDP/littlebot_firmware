/*
 * encoder.c
 *
 *  Created on: 22 de abr de 2018
 *      Author: nestor
 */

#include "app/encoder.h"


void EncoderConfigure(void){
	pulse_right=0;
	pulse_left=0;
	EncoderGpioRight();
	EncoderGpioLeft();
	EncoderTimerRight();
	EncoderTimerLeft();

}


void EncoderGpioRight(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		
	SysCtlDelay(3);

	//Definindo direção das entradas dos encoders
	GPIOPinTypeGPIOInput (GPIO_PORTB_BASE, ENCODER_PIN_DIR);
	GPIOPadConfigSet (GPIO_PORTB_BASE, ENCODER_PIN_DIR, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	//Configuração da interrupção para os encoders
	GPIOIntEnable (GPIO_PORTB_BASE, ENCODER_PIN_DIR);
	GPIOIntTypeSet (GPIO_PORTB_BASE, ENCODER_PIN_DIR, GPIO_RISING_EDGE);
	GPIOIntRegister (GPIO_PORTB_BASE, EncoderRightHandler);
}


void EncoderGpioLeft(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);		
	SysCtlDelay(3);
	
	//Definindo direção das entradas dos encoders
	GPIOPinTypeGPIOInput (GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPadConfigSet (GPIO_PORTE_BASE, ENCODER_PIN_DIR, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	//Configuração da interrupção para os encoders
	GPIOIntEnable (GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOIntTypeSet (GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
	GPIOIntRegister (GPIO_PORTE_BASE, EncoderLeftHandler);
}


void configura_WTIMER2_encoderDir(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	SysCtlDelay(3);

	//Comfigura o timer para o periodic timer mode
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_PERIODIC_UP);
	TimerEnable	  (WTIMER2_BASE, TIMER_A);
}


void EncoderTimerLeft(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
	SysCtlDelay(3);

	//Comfigura o timer para o periodic timer mode
	TimerConfigure(WTIMER1_BASE, TIMER_CFG_PERIODIC_UP);
	TimerEnable	  (WTIMER1_BASE, TIMER_A);
}


void EncoderRightHandler(){
	GPIOIntClear(ENCODER_GPIO_DIR, ENCODER_PIN_DIR);		//clear interrupt flag
	TimerDisable(WTIMER2_BASE,TIMER_A); 								//stop timer
	pulse_right = TimerValueGet(WTIMER2_BASE,TIMER_A); 		//record value
	//pulse = pulse/80000; //resultado em milesegundos

	pulse_right = pulse_right/80; 														//resultado em microsegundos

	HWREG(WTIMER2_BASE + TIMER_O_TAV) = 0; 							//Loads value 0 into the timer.
	TimerEnable(WTIMER2_BASE,TIMER_A); 									//start timer to recor
}


void EncoderLeftHandler(){
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);					//clear interrupt flag
	TimerDisable(WTIMER1_BASE,TIMER_A); 								//stop timer
	pulse_left = TimerValueGet(WTIMER1_BASE,TIMER_A); 		//record value
	//pulse = pulse/80000; //resultado em milesegundos

	pulse_left = pulse_left/80; 														//resultado em microsegundos

	HWREG(WTIMER1_BASE + TIMER_O_TAV) = 0; 							//Loads value 0 into the timer.
	TimerEnable(WTIMER1_BASE,TIMER_A); 									//start timer to recor
}



