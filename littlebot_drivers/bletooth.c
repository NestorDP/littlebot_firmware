#include "littlebot_drivers/bluetooth.h"


void BluetoothConfigure(void){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  SysCtlDelay(3);
  GPIOPinConfigure(GPIO_PB0_U1RX);
  GPIOPinConfigure(GPIO_PB1_U1TX);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
  SysCtlDelay(3);
  GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);
  UARTStdioConfig(1, 115200, 16000000);
}

uint32_t BluetoothRead(char *msg, uint32_t len){
  // uint32_t right_velocity;
  // uint32_t left_velocity;

  // char msg[40];
  // char *msg_split;

  UARTgets(msg, len);

  // msg_split = strtok(msg, "#"); // NOLINT
  // *right_vel = atoi(msg_split);

  // msg_split = strtok(NULL, "#"); // NOLINT
  // *left_vel = atoi(msg_split);
}

uint32_t BluetoothWrite(char *msg){
  UARTprintf("%s\n", "ssssssssssss");
  // return 2;
}
