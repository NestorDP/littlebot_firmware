// Copyright (c) 2022


#include "littlebot_firmware/task_motor_controller.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128
#define MOTOR_CONTROLLER_TASK_DELAY      100
#define LEFT  1
#define RIGHT 0

extern xQueueHandle g_pCommandVelLeftQueue;
extern xQueueHandle g_pStatusVelLeftQueue;
extern xQueueHandle g_pStatusPosLeftQueue;

extern xQueueHandle g_pCommandVelRightQueue;
extern xQueueHandle g_pStatusVelRightQueue;
extern xQueueHandle g_pStatusPosRightQueue;

const uint8_t side_left = LEFT;
const uint8_t side_right = RIGHT;


static void vTaskMotorController(void *pvParameters) {
  portTickType ui32WakeTime;
  // uint32_t ui32MotorTaskDelay; 

  float command_velocity = 0.0f;
  float status_velocity = 10.0f;
  float status_position = 20.0f;
  float controled_velocity = 0.0f;

  uint8_t *side_motor;
  side_motor = (uint8_t*)pvParameters;

  MotorWrapper motor_device;
  PidController pid;

  MotorWrapperConstruct(&motor_device, *side_motor);
  PidControllerConstruct(&pid, 11.0, 0.0, 0.0, MOTOR_CONTROLLER_TASK_DELAY/1000);
  pid.SetMaxSpeed(&pid, 22.0);

  // ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
  ui32WakeTime = xTaskGetTickCount();
  
  while(1) {
    status_velocity = motor_device.GetVelocity(&motor_device);
    status_position = motor_device.GetPosition(&motor_device);

    if(*side_motor == LEFT) {
      xQueueReceive(g_pCommandVelLeftQueue, &command_velocity, ( TickType_t ) 0);
      xQueueSend(g_pStatusVelLeftQueue, &status_velocity, 0);
      xQueueSend(g_pStatusPosLeftQueue, &status_position, 0);
    } else {
      xQueueReceive(g_pCommandVelRightQueue, &command_velocity, ( TickType_t ) 0);
      xQueueSend(g_pStatusVelRightQueue, &status_velocity, 0);
      xQueueSend(g_pStatusPosRightQueue, &status_position, 0);
    }

    controled_velocity = pid.Controller(&pid, command_velocity, status_velocity);
    motor_device.SetVelocity(&motor_device, controled_velocity);

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
