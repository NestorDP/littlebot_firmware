// Copyright (c) 2022


#include "littlebot_firmware/task_motor_controller.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128         // Stack size in words
#define MOTOR_CONTROLLER_TASK_DELAY      100

extern xQueueHandle g_pVelocityLeftQueue;
extern xQueueHandle g_pVelocityRightQueue;
extern xQueueHandle g_pFBVelocityLeftQueue;
extern xQueueHandle g_pFBVelocityRightQueue;

uint8_t side_left = 1;
uint8_t side_right = 0;

uint8_t side_m;

static void vTaskMotorController(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32MotorTaskDelay; 

    float velocity = 0;
    float feed_back_velocity = 10;  

    uint8_t *side_motor;
    side_motor = (uint8_t*)pvParameters;

    MotorInterface motor;
    
    MotorInterfaceConstruct(&motor, *side_motor);
    ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
    ui32WakeTime = xTaskGetTickCount();
    
    while(1) {
        feed_back_velocity = motor.GetVelocity(&motor);
        
        if(*side_motor == 1) {
            xQueueReceive(g_pVelocityLeftQueue, &velocity, ( TickType_t ) 10);
            xQueueSend(g_pFBVelocityLeftQueue, &feed_back_velocity, 0);
        } else {
            xQueueReceive(g_pVelocityRightQueue, &velocity, ( TickType_t ) 10);
            xQueueSend(g_pFBVelocityRightQueue, &feed_back_velocity, 0);
        }
        motor.SetVelocity(&motor, velocity);

        xTaskDelayUntil(&ui32WakeTime, ui32MotorTaskDelay / portTICK_RATE_MS);
    }    
}


uint32_t MotorControllerTaskInit(uint8_t side, const char *name, UBaseType_t priority) {
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
