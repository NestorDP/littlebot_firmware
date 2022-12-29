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
  int num_character;
  num_character = UARTPeek('\r');

  if ( num_character != -1) {
    UARTgets (msg, num_character);
  }
}

uint32_t UartPortPut (char *msg) {
  UARTprintf ("%s\n", msg);
  // return 2;
}
