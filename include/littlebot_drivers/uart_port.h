#ifndef INCLUDE_LITTLEBOT_DRIVER_UART_PORT_H__
#define INCLUDE_LITTLEBOT_DRIVER_UART_PORT_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#include "utils/uartstdio.h"


void UartPortConfigure(uint32_t baud_rate);
uint32_t UartPortRead(char *msg, uint32_t len);
uint32_t UartPortWrite(char *msg);

#endif // INCLUDE_LITTLEBOT_DRIVER_UART_PORT_H__
