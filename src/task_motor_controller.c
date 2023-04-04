// Copyright (c) 2022


#include "littlebot_firmware/task_motor_controller.h"

#define MOTOR_CONTROLLER_TASK_STACK_SIZE 128         // Stack size in words
#define MOTOR_CONTROLLER_TASK_DELAY      100

extern xQueueHandle g_pVelocityQueue;
extern xQueueHandle g_pFBVelocityQueue;

uint8_t side_m;

static void vTaskMotorController(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32MotorTaskDelay; 

    float velocity[2] = {0, 0};
    float feed_back_velocity[2] = {0, 0};  

    uint8_t *side_motor;
    side_motor = (uint8_t*)pvParameters;

    MotorInterface motor;
    
    MotorInterfaceConstruct(&motor, *side_motor);
    ui32MotorTaskDelay = MOTOR_CONTROLLER_TASK_DELAY ;
    ui32WakeTime = xTaskGetTickCount();
    
    while(1) {
        feed_back_velocity[motor.GetSide(&motor)]; // chage for GetVelocity
        
        xQueueReceive(g_pFBVelocityQueue, &feed_back_velocity, 0);
        xQueueSend(g_pFBVelocityQueue, &feed_back_velocity, portMAX_DELAY);
        xQueueReceive(g_pVelocityQueue, &velocity, 0);
        
        motor.SetVelocity(&motor, 20000);

        xTaskDelayUntil(&ui32WakeTime, ui32MotorTaskDelay / portTICK_RATE_MS);
    }    
}


uint32_t MotorControllerTaskInit(uint8_t side, const char *name, UBaseType_t priority) {
    side_m = side;
    if( xTaskCreate(vTaskMotorController, 
                    (const portCHAR *)name, 
                    MOTOR_CONTROLLER_TASK_STACK_SIZE, 
                    (void *) &side_m,
                    tskIDLE_PRIORITY + priority, 
                    NULL) != pdTRUE) {
        return(1);
    }

    // Success.
    return(0);
}
