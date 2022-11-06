#include "littlebot_api/api_motor.h"

static void ConfigMotor (LittlebotMotor *self, uint8_t side){
  MotorConfigure();
  self->side = side;
  MotorVelocity(1, 23000, 1);
}


static void SetLinVelocit (LittlebotMotor *self, uint16_t vel, uint8_t dir) {
  MotorVelocity(self->side, vel, dir);
}
