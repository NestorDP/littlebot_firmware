/* Copyright (c) 2022-2025 Nestor D. Pereira Neto

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_SERIAL_WRAPPER_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_SERIAL_WRAPPER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "littlebot_drivers/uart_port.h"

typedef struct SerialWrapperClass SerialWrapper;

typedef uint32_t (*ptfRead)(char *msg);
typedef void (*ptfWrite)(char *msg);

//structure of function pointer
struct SerialWrapperClass{
  ptfRead Read;
  ptfWrite Write;
};

void SerialWrapperConstructor(SerialWrapper *self, uint32_t baud_rate);

uint32_t fcRead(char *msg);

void fcWrite(char *msg);


#endif // INCLUDE_LITTLEBOT_FIRMWARE_SERIAL_WRAPPER_H__
