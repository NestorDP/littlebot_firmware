#include "littlebot_drivers/motor.h"


void MotorConfigure(void){
  //Configure PWM
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  GPIOPinConfigure(GPIO_PC4_M0PWM6);
  GPIOPinConfigure(GPIO_PC5_M0PWM7);
  GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4);
  GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);
  PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 25000);
  PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
  PWMGenEnable(PWM0_BASE, PWM_GEN_3);

  // Configure GPIO
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)){}
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
  GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
}

uint8_t MotorVelocity(uint8_t side, uint16_t vel, uint8_t dir){
  if(side == LEFT){
    if(dir == FORWARD){
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
    else{
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0);
    }
    PWMPulseWidthSet(PWM0_BASE, MOTOR_LEFT_PWM_OUT, vel);
  }
  else{
    if(dir == FORWARD){
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
    else{
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
    }
    PWMPulseWidthSet(PWM0_BASE, MOTOR_RIGHT_PWM_OUT, vel);
  }

  return dir;
}
