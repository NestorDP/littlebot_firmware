#include "littlebot_firmware/motor_interface.h"


void MotorInterfaceConstruct(MotorInterface *self, uint8_t side){
  self->motor_side_ = side;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
  self->GetPosition = fcGetPosition;
  self->GetSide = fcGetSide;

  if (self->motor_side_ == 1){
    EncoderLeftConfigure();
  } else {
    EncoderRightConfigure();
  }

  MotorConfigure();
}


void fcSetVelocity(MotorInterface *self, float velocity){
  if(velocity < 0){
    MotorVelocity(self->motor_side_, -1 * velocity, 1);
    EncoderSetPositionDirection(self->motor_side_, 1);
    self->motor_direction_ = 1;
  } else{
    MotorVelocity(self->motor_side_, velocity, 0);
    EncoderSetPositionDirection(self->motor_side_, 0);
    self->motor_direction_ = 0;
  }
}


float fcGetVelocity(MotorInterface *self) {
  if (self->motor_side_ == 1) {
    return EncoderVelocityRightValue();
  } else {
    return EncoderVelocityLeftValue();
  }
}


float fcGetPosition(MotorInterface *self) {
  if (self->motor_side_ == 1) {
    return EncoderPositionRightValue();
  } else {
    return EncoderPositionLeftValue();
  }
  
}


uint8_t fcGetSide(MotorInterface *self){
  return self->motor_side_;
}
