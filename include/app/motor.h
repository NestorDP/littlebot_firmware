#ifndef __APP_MOTOR_H__
#define __APP_MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

// PWM configutations
#define MOTOR_LEFT_PWM_OUT      PWM_OUT_6
#define MOTOR_LEFT_PWM_CONFIG   GPIO_PC5_M0PWM6
#define MOTOR_LEFT_PWM_PIN      GPIO_PIN_4

#define MOTOR_RIGHT_PWM_OUT     PWM_OUT_7
#define MOTOR_RIGHT_PWM_CONFIG  GPIO_PC5_M0PWM7
#define MOTOR_RIGHT_PWM_PIN     GPIO_PIN_5

// GPIO pins configurations


void MotorConfigure(void);

#endif // __APP_MOTOR_H__
