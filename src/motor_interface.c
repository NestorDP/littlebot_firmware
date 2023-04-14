#include "littlebot_firmware/motor_interface.h"


void MotorInterfaceConstruct(MotorInterface *self, uint8_t side){
  self->motor_side_ = side;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
  self->GetSide = fcGetSide;

  // EncoderConfigure();
  MotorConfigure();
}


void fcSetVelocity(MotorInterface *self, int16_t vel){
  if(vel < 0){
    MotorVelocity(self->motor_side_, -1 * vel, 1);
  } else{
    MotorVelocity(self->motor_side_, vel, 0);
  }
}


float fcGetVelocity(MotorInterface *self) {

}

uint8_t fcGetSide(MotorInterface *self){
  return self->motor_side_;
}
