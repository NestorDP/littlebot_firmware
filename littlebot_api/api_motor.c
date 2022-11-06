#include "littlebot_api/api_motor.h"


void MotorInterfaceContruct(MotorInterface *self, uint8_t side){
  self->motor_side_ = side;
  self->ConfigMotor = MotorConfigure;
  self->SetLinVelocit = fcSetLinVelocit;

}

void fcSetLinVelocit(MotorInterface *self, uint16_t vel, uint8_t dir){
  MotorVelocity(self->motor_side_, vel, dir);
}
