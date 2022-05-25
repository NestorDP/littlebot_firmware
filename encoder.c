/*
 * encoder.c
 *
 *  Created on: 22 de abr de 2018
 *      Author: nestor
 */

#include "app/encoder.h"



void EncoderConfigure(void){
	// pulse_right=0;
	// pulse_left=0;
	EncoderGpioRight();
	EncoderGpioLeft();
	EncoderTimerRight();
	EncoderTimerLeft();
}


void EncoderGpioRight(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);		
	SysCtlDelay(3);

	// Definindo direção das entradas dos encoders
	GPIOPinTypeGPIOInput (GPIO_PORTA_BASE, GPIO_PIN_5);
	GPIOPadConfigSet (GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	// Configuração da interrupção para os encoders
	GPIOIntEnable (GPIO_PORTA_BASE, GPIO_PIN_5);
	GPIOIntTypeSet (GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_RISING_EDGE);
	GPIOIntRegister (GPIO_PORTA_BASE, EncoderRightHandler);
}


void EncoderGpioLeft(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);		
	SysCtlDelay(3);
	
	// Definindo direção das entradas dos encoders
	GPIOPinTypeGPIOInput (GPIO_PORTE_BASE, GPIO_PIN_3);
	GPIOPadConfigSet (GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	// Configuração da interrupção para os encoders
	GPIOIntEnable (GPIO_PORTE_BASE, GPIO_PIN_3);
	GPIOIntTypeSet (GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_RISING_EDGE);
	GPIOIntRegister (GPIO_PORTE_BASE, EncoderLeftHandler);
}


void EncoderTimerRight(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	SysCtlDelay(3);

	// Comfigura o timer para o periodic timer mode
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_PERIODIC_UP);
	TimerEnable	  (WTIMER2_BASE, TIMER_A);
}


void EncoderTimerLeft(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
	SysCtlDelay(3);

	// Comfigura o timer para o periodic timer mode
	TimerConfigure(WTIMER1_BASE, TIMER_CFG_PERIODIC_UP);
	TimerEnable	  (WTIMER1_BASE, TIMER_A);
}


void EncoderRightHandler(){
	volatile uint32_t pulse_right;

	// clear interrupt flag
	GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_5);

	// stop timer		
	TimerDisable(WTIMER2_BASE,TIMER_A); 

	// record value								
	pulse_right = TimerValueGet(WTIMER2_BASE,TIMER_A); 

	// resultado em milesegundos	
	// pulse = pulse/80000; 

	// resultado em microsegundos
	pulse_right = pulse_right/80;
	UARTprintf("\nola ENCODER!!!right \t%d \n", pulse_right);													

	// Loads value 0 into the timer.
	HWREG(WTIMER2_BASE + TIMER_O_TAV) = 0;

	// start timer to recor						
	TimerEnable(WTIMER2_BASE,TIMER_A); 									
}


void EncoderLeftHandler(){
	volatile uint32_t pulse_left;

	// clear interrupt flag
	GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_3);

	// stop timer				
	TimerDisable(WTIMER1_BASE,TIMER_A);

	// record value
	pulse_left = TimerValueGet(WTIMER1_BASE,TIMER_A);
	
	// resultado em milesegundos
	// pulse = pulse/80000;

	// resultado em microsegundos
	pulse_left = pulse_left/80;
	UARTprintf("ola ENCODER!!! left \t%d \r", pulse_left);

	// Loads value 0 into the timer.
	HWREG(WTIMER1_BASE + TIMER_O_TAV) = 0;

	// start timer to recor
	TimerEnable(WTIMER1_BASE,TIMER_A);
}



