// Copyright (c) 2022-2023 Nestor D. Pereira Neto

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
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
}

uint8_t MotorVelocity(uint8_t side, float normalized_vel, uint8_t direction){
  uint16_t pwm_vel;

  pwm_vel = (normalized_vel*10000) + 14000;
  if (pwm_vel == 14000) {
    pwm_vel = 0;
  }

  if(side == LEFT){
    if(direction == FORWARD){
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
    else{
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
      GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0);
    }
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, pwm_vel);
  }
  else{
    if(direction == FORWARD){
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);
    }
    else{
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
      GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
    }
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, pwm_vel);
  }

  return direction;
}
