

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_TASK_SERIAL_READ_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_TASK_SERIAL_READ_H__

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "littlebot_firmware/serialization.h"
#include "littlebot_firmware/serial.h"
#include "littlebot_firmware/priorities.h"


uint32_t SerialReadTaskInit(void);

#endif // INCLUDE_LITTLEBOT_FIRMWARE_TASK_SERIAL_READ_H__
