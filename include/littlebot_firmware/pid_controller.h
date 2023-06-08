// Copyright (c) 2023 Nestor D. Pereira Neto

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

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_PID_CONTROLER_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_PID_CONTROLER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct PidControllerClass PidController;
typedef struct Gain pidGain;

typedef float (*ptfController)(PidController *self, float set_point, float feed_back);
typedef void (*ptfSetMaxSpeed)(PidController *self, float max);

struct Gain{
    float p;
    float d;
    float i;
};

//structure of function pointer
struct PidControllerClass{
  // Variables
  pidGain gain_;
  float max_speed_;
  float time_sample_;

  // Function
  ptfController Controller;
  ptfSetMaxSpeed SetMaxSpeed;
};


void PidControllerConstruct(PidController *self, float kp, float kd, float ki, float t);

float fcController(PidController *self, float set_point, float feed_back);

void fcSetMaxSpeed(PidController *self, float speed);


#endif // INCLUDE_LITTLEBOT_FIRMWARE_PID_CONTROLER_H__
