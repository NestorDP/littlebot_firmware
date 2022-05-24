#ifndef __APP_BLUETOOTH_H__
#define __APP_BLUETOOTH_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "stdlib.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"


void BluetoothConfigure(void);

#endif // __APP_BLUETOOTH_H__
