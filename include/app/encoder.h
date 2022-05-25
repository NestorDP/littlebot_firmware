/*
 * encoder.h
 *
 *  Created on: 22 de abr de 2018
 *      Author: nestor
 */


#ifndef __APP_ENCODER_H__
#define __APP_ENCODER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"

#define ENCODER_PERIPH_DIR 	SYSCTL_PERIPH_GPIOB
#define ENCODER_GPIO_DIR	GPIO_PORTB_BASE
#define ENCODER_PIN_DIR		GPIO_PIN_2


void EncoderConfigure(void);

void EncoderGpioRight(void);
void EncoderGpioLeft(void);
void EncoderTimerRight(void);
void EncoderTimerLeft(void);

void EncoderRightHandler();
void EncoderLeftHandler();

// volatile uint32_t pulse_right;
// volatile uint32_t pulse_left;

#endif // __APP_ENCODER_H__