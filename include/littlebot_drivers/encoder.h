// Copyright (c) 2018-2023 Nestor D. Pereira Neto

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

#ifndef INCLUDE_LITTLEBOT_DRIVER_ENCODER_H__
#define INCLUDE_LITTLEBOT_DRIVER_ENCODER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_qei.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"

#define PERIODS_PER_SECOND    2
#define PULSES_PER_REVOLUTION 40
#define _2PI                  6.2831853f
#define ANGLE_STEP            _2PI / PULSES_PER_REVOLUTION

void EncoderInit(void);

void EncoderRightConfigure(void);
void EncoderLeftConfigure(void);

float EncoderVelocityRightValue(void);
float EncoderVelocityLeftValue(void);

float EncoderPositionRightValue(void);
float EncoderPositionLeftValue(void);

void EncoderRightHandler(void);
void EncoderLeftHandler(void);

void EncoderSetPositionDirection(uint8_t side, uint8_t direction);

static volatile uint32_t ui32EncoderVelRight;
static volatile uint32_t ui32EncoderVelLeft;

static volatile uint32_t ui32EncoderPosRight;
static volatile uint32_t ui32EncoderPosLeft;

#endif // INCLUDE_LITTLEBOT_DRIVER_ENCODER_H__
