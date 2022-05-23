#include "app/motor.h"


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

}

