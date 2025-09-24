// Copyright (c) 2022


#include "littlebot_firmware/task_motor_controller.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128         // Stack size in words
#define MOTOR_CONTROLLER_TASK_DELAY      100

extern xQueueHandle g_pCommandVelLeftQueue;
extern xQueueHandle g_pStatusVelLeftQueue;
extern xQueueHandle g_pStatusPosLeftQueue;

extern xQueueHandle g_pCommandVelRightQueue;
extern xQueueHandle g_pStatusVelRightQueue;
extern xQueueHandle g_pStatusPosRightQueue;

uint8_t side_left = 1;
uint8_t side_right = 0;

uint8_t side_m;


static void vTaskMotorController(void *pvParameters) {
  portTickType ui32WakeTime;
  // uint32_t ui32MotorTaskDelay; 

  float velocity = 0;
  float feed_back_velocity = 10;
  float feed_back_position = 20;
  float controled_velocity = 0.0;

  uint8_t *side_motor;
  side_motor = (uint8_t*)pvParameters;

  MotorInterface motor;
  PidController pid;
  
  MotorInterfaceConstruct(&motor, *side_motor);
  PidControllerConstruct(&pid, 11.0, 0.0, 0.0, MOTOR_CONTROLLER_TASK_DELAY/1000);
  pid.SetMaxSpeed(&pid, 22.0);

  // ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
  ui32WakeTime = xTaskGetTickCount();
  
  while(1) {
    feed_back_velocity = motor.GetVelocity(&motor);
    feed_back_position = motor.GetPosition(&motor);
    
    if(*side_motor == 1) {
      xQueueReceive(g_pCommandVelLeftQueue, &velocity, ( TickType_t ) 0);
      xQueueSend(g_pStatusVelLeftQueue, &feed_back_velocity, 0);
      xQueueSend(g_pStatusPosLeftQueue, &feed_back_position, 0);
    } else {
      xQueueReceive(g_pCommandVelRightQueue, &velocity, ( TickType_t ) 0);
      xQueueSend(g_pStatusVelRightQueue, &feed_back_velocity, 0);
      xQueueSend(g_pStatusPosRightQueue, &feed_back_position, 0);
    }

    controled_velocity = pid.Controller(&pid, velocity, feed_back_velocity);
    motor.SetVelocity(&motor, velocity);

    xTaskDelayUntil(&ui32WakeTime,  pdMS_TO_TICKS(MOTOR_CONTROLLER_TASK_DELAY));
  }    
}


uint32_t MotorControllerTaskInit(uint8_t side, const char *name) {
  if (side){
    if( xTaskCreate(vTaskMotorController, 
                    (const portCHAR *)name, 
                    MOTOR_CONTROLLER_TASK_STACK_SIZE, 
                    (void *) &side_left,
                    tskIDLE_PRIORITY + PRIORITY_LEFT_MOTOR_TASK,
                    NULL) != pdTRUE) {
      return(1);
    }
  } else {
    if( xTaskCreate(vTaskMotorController, 
                    (const portCHAR *)name, 
                    MOTOR_CONTROLLER_TASK_STACK_SIZE, 
                    (void *) &side_right,
                    tskIDLE_PRIORITY + PRIORITY_RIGHT_MOTOR_TASK, 
                    NULL) != pdTRUE) {
      return(1);
    }
  }

  // Success.
  return(0);
}
