#ifndef INCLUDE_LITTLEBOT_DRIVER_BLUETOOTH_H__
#define INCLUDE_LITTLEBOT_DRIVER_BLUETOOTH_H__

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


void BluetoothConfigure(void);
uint32_t BluetoothRead(char *msg, uint32_t len);
uint32_t BluetoothWrite(char *msg);

#endif // INCLUDE_LITTLEBOT_DRIVER_BLUETOOTH_H__
