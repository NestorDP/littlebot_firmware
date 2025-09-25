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

#include "littlebot_drivers/uart_port.h"

void UartPortConfigure (uint32_t baud_rate, uint8_t type) {
    switch (type) {
    case DATA_STREAM:
        /* Enable GPIO port B which is used for UART1 pins. */
        SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOB);

        /* Enable UART1 so that we can configure the clock. */
        SysCtlPeripheralEnable (SYSCTL_PERIPH_UART1);

        /* Configure the pin muxing for UART1 functions on port B0 and B1.
        * This step is not necessary if your part does not support pin muxing. */
        GPIOPinConfigure (GPIO_PB0_U1RX);
        GPIOPinConfigure (GPIO_PB1_U1TX);

        /* Select the alternate (UART) function for these pins. */
        GPIOPinTypeUART (GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        /* Use the internal 16MHz oscillator as the UART clock source. */
        UARTClockSourceSet (UART1_BASE, UART_CLOCK_PIOSC);

        /* Configure UART1 parameters manually */
        UARTConfigSetExpClk(UART1_BASE, 16000000, baud_rate, 
                           (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
        
        /* Enable the UART */
        UARTEnable(UART1_BASE);
        break;
    
    case DEBUG_CONSOLE:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTStdioConfig(0, baud_rate, SysCtlClockGet());
        break;
    }
}


int32_t UartPortRead(char *buffer) {
    int32_t index = 0;
    int32_t max_buffer_length = 128;

    while (index < max_buffer_length && UARTCharsAvail(UART1_BASE)) {
        int32_t received_char = UARTCharGet(UART1_BASE);
        
        // Check for error (UARTCharGet returns -1 on error)
        if (received_char < 0) {
            return -1;  // Error occurred, stop reading
        }
        
        buffer[index] = (char)(received_char & 0xFF);  // Safe conversion  
        if (buffer[index] == '\n') {
            index++;
            break;
        }
        index++;
    }

    if (index < max_buffer_length) {
        buffer[index] = '\0';
    } else {
        buffer[max_buffer_length - 1] = '\0';
    }
    
    return index;
}


int32_t UartPortWrite (char *msg, size_t length) {
  uint32_t index;
  
  for(index = 0; index < length; index++)
  {
      UARTCharPut(UART1_BASE, msg[index]);
  }
  UARTCharPut(UART1_BASE, '\n');
  return 0;
}


int32_t UartPortPrintf (char *msg) {
    UARTprintf(msg);
    return 0;
}
