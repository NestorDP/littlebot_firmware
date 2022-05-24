
#include "app/bluetooth.h"


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

// uint32_t BluetoothRead(uint16_t* right_vel, uint16_t* left_vel){

// }

uint32_t BluetoothWrite(uint16_t *right_vel, uint16_t* left_vel){
  UARTprintf("%d#%d\n", *right_vel, *left_vel);
  return 2;
}
