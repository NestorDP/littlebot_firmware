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

typedef struct MotorWrapperClass MotorWrapper;

typedef void (*ptfSetVelocity)(MotorWrapper *self, float vel);
typedef float (*ptfGetVelocity)(MotorWrapper *self);
typedef float (*ptfGetPosition)(MotorWrapper *self);
typedef uint8_t (*ptfGetSide)(MotorWrapper *self);

//structure of function pointer
struct MotorWrapperClass{
    // Variables
    uint8_t motor_side_;
    uint8_t motor_direction_;

    // Function
    ptfSetVelocity SetVelocity;
    ptfGetVelocity GetVelocity;
    ptfGetPosition GetPosition;
    ptfGetSide GetSide;
};

void MotorWrapperConstruct(MotorWrapper *self, uint8_t side);

void fcSetVelocity(MotorWrapper *self, float velocity);

float fcGetVelocity(MotorWrapper *self);

float fcGetPosition(MotorWrapper *self);

uint8_t fcGetSide(MotorWrapper *self);


#endif // INCLUDE_LITTLEBOT_FIRMWARE_WHEEL_CONTROL_H__
