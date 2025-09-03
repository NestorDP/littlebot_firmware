// Copyright (c) 2022-2023 Nestor D. Pereira Neto

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

#include "littlebot_drivers/uart_port.h"

void UartPortConfigure (uint32_t baud_rate) {
  uint32_t baud = baud_rate;

    SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable (SYSCTL_PERIPH_UART1);
    GPIOPinConfigure (GPIO_PB0_U1RX);
    GPIOPinConfigure (GPIO_PB1_U1TX);
    GPIOPinTypeUART (GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet (UART1_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig (1, baud, 16000000);
}

uint32_t UartPortGet (char *msg) {
  uint32_t num_character;
  int num;
  num = UARTPeek('\r');
  num_character = (uint32_t)num;
  if ( num > 0) {
    UARTgets (msg, num_character + 1);
    num = 0;
  }
  
  return num_character;
}

uint32_t UartPortPut (char *msg) {
  UARTprintf ("%s\n", msg);
  return 0;
}
