#include "littlebot_drivers/uart_port.h"

void UartPortConfigure(uint32_t baud_rate, uint8_t port) {
  uint32_t baud = baud_rate;

  switch (port)
  {
  case 0:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(3);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlDelay(3);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
    break;
  case 1:
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    SysCtlDelay(3);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    SysCtlDelay(3);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(1, baud, 16000000);
    break;
  default:
    break;
  }
}

uint32_t UartPortRead(char *msg, uint32_t len){
  UARTgets(msg, len);
}

uint32_t UartPortWrite(char *msg){
  UARTprintf("%s\n", msg);
  // return 2;
}
