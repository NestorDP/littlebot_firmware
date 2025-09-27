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

#include "littlebot_firmware/task_motor_controller.h"
#include "littlebot_firmware/types.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128
#define MOTOR_CONTROLLER_TASK_DELAY      150

extern QueueHandle_t g_pCommandVelLeftQueue;
extern QueueHandle_t g_pStatusVelLeftQueue;
extern QueueHandle_t g_pStatusPosLeftQueue;

extern QueueHandle_t g_pCommandVelRightQueue;
extern QueueHandle_t g_pStatusVelRightQueue;
extern QueueHandle_t g_pStatusPosRightQueue;

extern xSemaphoreHandle g_pUartLoggerSemaphore;

extern SerialWrapper console;

const uint8_t side_left = LEFT;
const uint8_t side_right = RIGHT;


static void vTaskMotorController(void *pvParameters) {
	portTickType ui32WakeTime;
	// uint32_t ui32MotorTaskDelay; 


	WheelData_t wheel_data;

	uint8_t *side_motor;
	side_motor = (uint8_t*)pvParameters;

	MotorWrapper motor_device;
	PidController pid;

	MotorWrapperConstruct(&motor_device, *side_motor);
	// PidControllerConstruct(&pid, 11.0, 0.0, 0.0, MOTOR_CONTROLLER_TASK_DELAY/1000);
	// pid.SetMaxSpeed(&pid, 22.0);

	// ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
	ui32WakeTime = xTaskGetTickCount();

	if (*side_motor == 0) {
		xSemaphoreTake(g_pUartLoggerSemaphore, portMAX_DELAY);
		console.Printf("Left Motor Controller task started\n");
		xSemaphoreGive(g_pUartLoggerSemaphore);
	} else {
		xSemaphoreTake(g_pUartLoggerSemaphore, portMAX_DELAY);
		console.Printf("Right Motor Controller task started\n");
		xSemaphoreGive(g_pUartLoggerSemaphore);
	}
	
	while(1) {
		wheel_data.status_velocity = motor_device.GetVelocity(&motor_device);
		wheel_data.status_position = motor_device.GetPosition(&motor_device);

		if(*side_motor == 0) {
		xQueueReceive(g_pCommandVelLeftQueue, &wheel_data.command_velocity, ( TickType_t )  0);
		xQueueSend(g_pStatusVelLeftQueue, &wheel_data.status_velocity, 0);
		xQueueSend(g_pStatusPosLeftQueue, &wheel_data.status_position, 0);
		} else {
		xQueueReceive(g_pCommandVelRightQueue, &wheel_data.command_velocity, ( TickType_t ) 0);
		xQueueSend(g_pStatusVelRightQueue, &wheel_data.status_velocity, 0);
		xQueueSend(g_pStatusPosRightQueue, &wheel_data.status_position, 0);
		}

		// controled_velocity = pid.Controller(&pid, command_velocity, status_velocity);
		motor_device.SetVelocity(&motor_device, wheel_data.command_velocity);

		xTaskDelayUntil(&ui32WakeTime,  pdMS_TO_TICKS(MOTOR_CONTROLLER_TASK_DELAY));
	}    
}


uint32_t MotorControllerTaskInit(uint8_t side, const char *name) {
  switch (side){
  case LEFT:
    if( xTaskCreate(vTaskMotorController, 
                    (const portCHAR *)name, 
                    MOTOR_CONTROLLER_TASK_STACK_SIZE, 
                    (void *) &side_left,
                    tskIDLE_PRIORITY + PRIORITY_LEFT_MOTOR_TASK,
                    NULL) != pdTRUE) {
      return(1);
    }
    break;
  case RIGHT:
    if( xTaskCreate(vTaskMotorController, 
                    (const portCHAR *)name, 
                    MOTOR_CONTROLLER_TASK_STACK_SIZE, 
                    (void *) &side_right,
                    tskIDLE_PRIORITY + PRIORITY_RIGHT_MOTOR_TASK,
                    NULL) != pdTRUE) {
      return(1);
    }
    break;
  } 
  /* Success. */
  return(0);
}
