/*
 * encoder.h
 *
 *  Created on: 22 de abr de 2018
 *      Author: nestor
 */


#ifndef INCLUDE_LITTLEBOT_DRIVER_ENCODER_H__
#define INCLUDE_LITTLEBOT_DRIVER_ENCODER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/qei.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"
#include "inc/hw_qei.h"


#define SAMPLE_PERIOD  8
#define PPR            40


void EncoderInit(void);

void EncoderRightConfigure(void);
// void EncoderLeftConfigure(void);

void EncoderTimerRight(void);
// void EncoderTimerLeft(void);

uint32_t EncoderGetRightValue(void);
// uint32_t EncoderGetLeftValue(void);

void EncoderRightHandler(void);
// void EncoderLeftHandler();

static volatile uint32_t ui32EncoderVelRight;
static volatile uint32_t flag = 0x0;

#endif // INCLUDE_LITTLEBOT_DRIVER_ENCODER_H__
