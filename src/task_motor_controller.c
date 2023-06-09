// Copyright (c) 2022


#include "littlebot_firmware/task_motor_controller.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128         // Stack size in words
#define MOTOR_CONTROLLER_TASK_DELAY      100

extern xQueueHandle g_pVelocityLeftQueue;
extern xQueueHandle g_pVelocityRightQueue;
extern xQueueHandle g_pFBVelocityLeftQueue;
extern xQueueHandle g_pFBVelocityRightQueue;
extern xQueueHandle g_pFBPositionLeftQueue;
extern xQueueHandle g_pFBPositionRightQueue;

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
  PidControllerConstruct(&pid, 0.03, 0.02, 0.005, MOTOR_CONTROLLER_TASK_DELAY);
  pid.SetMaxSpeed(&pid, 15.0);

  // ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
  ui32WakeTime = xTaskGetTickCount();
  
  while(1) {
    feed_back_velocity = motor.GetVelocity(&motor);
    feed_back_position = motor.GetPosition(&motor);
    
    if(*side_motor == 1) {
      xQueueReceive(g_pVelocityLeftQueue, &velocity, ( TickType_t ) 0);
      xQueueSend(g_pFBVelocityLeftQueue, &feed_back_velocity, 0);
      xQueueSend(g_pFBPositionLeftQueue, &feed_back_position, 0);
    } else {
      xQueueReceive(g_pVelocityRightQueue, &velocity, ( TickType_t ) 0);
      xQueueSend(g_pFBVelocityRightQueue, &feed_back_velocity, 0);
      xQueueSend(g_pFBPositionRightQueue, &feed_back_position, 0);
    }

    controled_velocity = pid.Controller(&pid, velocity, feed_back_velocity);
    motor.SetVelocity(&motor, controled_velocity);

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
