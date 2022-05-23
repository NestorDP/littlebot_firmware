#ifndef __APP_MOTOR_H__
#define __APP_MOTOR_H__

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

#define MOTOR_LEFT_OUT PWM_OUT_6
#define MOTOR_RIGHT_OUT PWM_OUT_7

void MotorConfigure(void);

#endif // __APP_MOTOR_H__
