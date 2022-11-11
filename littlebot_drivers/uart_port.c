#include "littlebot_drivers/uart_port.h"

void UartPortConfigure(uint32_t baud_rate) {
  uint32_t baud = baud_rate;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(1, baud, 16000000);
}

uint32_t UartPortRead(char *msg, uint32_t len){
  UARTgets(msg, len);
}

uint32_t UartPortWrite(char *msg){
  UARTprintf("%s\n", msg);
  // return 2;
}
