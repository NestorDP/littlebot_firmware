#include "littlebot_drivers/motor.h"


void MotorConfigure(void){
  //Configure PWM
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

  GPIOPinConfigure(GPIO_PB4_M0PWM2);
  GPIOPinConfigure(GPIO_PB5_M0PWM3);

	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);

  PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 25000);
  PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT | PWM_OUT_2_BIT, true);
  PWMGenEnable(PWM0_BASE, PWM_GEN_1);

  // Configure GPIO
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}

  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_4);
}

uint8_t MotorVelocity(uint8_t side, uint16_t vel, uint8_t dir){
  if(side == LEFT){
    if(dir == FORWARD){
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_PIN_4);
    }
    else{
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, 0);
    }
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, vel);
  }
  else{
    if(dir == FORWARD){
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
    else{
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
    }
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, vel);
  }

  return dir;
}
