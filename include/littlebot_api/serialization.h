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

#ifndef INCLUDE_LITTLEBOT_API_SERIALIZATION_H__
#define INCLUDE_LITTLEBOT_API_SERIALIZATION_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "littlebot_api/serial.h"

typedef struct SerializationClass Serialization;

typedef void (*ptfReceiveMessage)(Serialization *self);
typedef void (*ptfSendMessage)(Serialization *self);
typedef void (*ptfSetVelocity)(Serialization *self, float vel);
typedef float (*ptfGetVelocity)(Serialization *self);

//structure of function pointer
struct SerializationClass{
  float encoder_velocity;
  float motor_velocity;
  char serialized_msg[200];
  SerialInterface serial;
  ptfReceiveMessage ReceiveMessage;
  ptfSendMessage SendMessage;
  ptfSetVelocity SetVelocity;
  ptfGetVelocity GetVelocity
};

void SerializationConstruct (Serialization *self);

void fcReceiveMessage (Serialization *self);

void fcSendMessage (Serialization *self);

void fcSetVelocity (Serialization *self, float vel);

float fcGetVelocity (Serialization *self);


#endif // INCLUDE_LITTLEBOT_API_SERIALIZATION_H__
