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

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_SERIALIZATION_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_SERIALIZATION_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "littlebot_firmware/serial.h"

typedef struct SerializationClass Serialization;

typedef void (*ptfReceiveMessage)(Serialization *self, float *left_vel, float *right_vel);
typedef void (*ptfSendMessage)(Serialization *self, float *left_vel, float *right_vel);


//structure of function pointer
struct SerializationClass{
  
  SerialInterface serial;
  ptfReceiveMessage ReceiveMessage;
  ptfSendMessage SendMessage
};

// Functions
void SerializationConstruct (Serialization *self);

void fcReceiveMessage (Serialization *self, float *left_vel, float *right_vel);

void fcSendMessage (Serialization *self, float *left_vel, float *right_vel);


// Variables
static char var_float[sizeof(float)];

static uint8_t i;



#endif // INCLUDE_LITTLEBOT_FIRMWARE_SERIALIZATION_H__
