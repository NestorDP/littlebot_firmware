#include "littlebot_firmware/motor_wrapper.h"


void MotorWrapperConstruct(MotorWrapper *self, uint8_t side){
  self->motor_side_ = side;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
  self->GetPosition = fcGetPosition;
  self->GetSide = fcGetSide;

  if (self->motor_side_ == LEFT){
    EncoderLeftConfigure();
  } else {
    EncoderRightConfigure();
  }

  MotorConfigure();
}


void fcSetVelocity(MotorWrapper *self, float velocity){
  if(velocity < 0){
    MotorVelocity(self->motor_side_, -1 * velocity, 1);
    EncoderSetPositionDirection(self->motor_side_, 1);
    self->motor_direction_ = BACKWARD;
  } else{
    MotorVelocity(self->motor_side_, velocity, 0);
    EncoderSetPositionDirection(self->motor_side_, 0);
    self->motor_direction_ = FORWARD;
  }
}


float fcGetVelocity(MotorWrapper *self) {
  if (self->motor_side_ == RIGHT) {
    return EncoderVelocityRightValue();
  } else {
    return EncoderVelocityLeftValue();
  }
}


float fcGetPosition(MotorWrapper *self) {
  if (self->motor_side_ == RIGHT) {
    return EncoderPositionRightValue();
  } else {
    return EncoderPositionLeftValue();
  }
  
}


uint8_t fcGetSide(MotorWrapper *self){
  return self->motor_side_;
}
