// Copyright (c) 2022 Nestor D. Pereira Neto

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

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_WHEEL_CONTROL_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_WHEEL_CONTROL_H__

#include <stdint.h>
#include <stdbool.h>

#include "littlebot_drivers/motor.h"
#include "littlebot_drivers/encoder.h"

typedef struct MotorInterfaceClass MotorInterface;

typedef void (*ptfSetVelocity)(MotorInterface *self, int16_t vel);
typedef float (*ptfGetVelocity)(MotorInterface *self);
typedef uint8_t (*ptfGetSide)(MotorInterface *self);

//structure of function pointer
struct MotorInterfaceClass{
    uint8_t motor_side_;
    ptfSetVelocity SetVelocity;
    ptfGetVelocity GetVelocity;
    ptfGetSide GetSide;
};

void MotorInterfaceConstruct(MotorInterface *self, uint8_t side);

void fcSetVelocity(MotorInterface *self, int16_t vel);

float fcGetVelocity(MotorInterface *self);

uint8_t fcGetSide(MotorInterface *self);


#endif // INCLUDE_LITTLEBOT_FIRMWARE_WHEEL_CONTROL_H__
