#include "littlebot_firmware/motor_interface.h"


void MotorInterfaceConstruct(MotorInterface *self, uint8_t side){
  self->motor_side_ = side;
  self->ConfigMotor = MotorConfigure;
  self->SetVelocit = fcSetVelocit;
  self->GetSide = fcGetSide;
}

void fcSetVelocit(MotorInterface *self, uint16_t vel, uint8_t dir){
  MotorVelocity(self->motor_side_, vel, dir);
}

uint8_t fcGetSide(MotorInterface *self){
  return self->motor_side_;
}
