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

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_SERIAL_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_SERIAL_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "littlebot_drivers/uart_port.h"

typedef struct SerialInterfaceClass SerialInterface;

typedef uint32_t (*ptfRead)(SerialInterface *self, char *msg);
typedef void (*ptfWrite)(SerialInterface *self, char *msg);

//structure of function pointer
struct SerialInterfaceClass{
  ptfRead Read;
  ptfWrite Write;
};

void SerialInterfaceContruct(SerialInterface *self, uint32_t baud_rate);

uint32_t fcRead(SerialInterface *self, char *msg);

void fcWrite(SerialInterface *self, char *msg);


#endif // INCLUDE_LITTLEBOT_FIRMWARE_SERIAL_H__
