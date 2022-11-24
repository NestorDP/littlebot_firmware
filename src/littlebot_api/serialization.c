#include "littlebot_api/serialization.h"

void SerializationConstruct(Serialization *self) {
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
  self->feedback_velocity[left] = 0;
  self->feedback_velocity[right] = 0;
  self->velocity[left] = 0;
  self->velocity[right] = 0;
  SerialInterfaceContruct(&self->serial, 115200);
}


void fcReceiveMessage (Serialization *self) {
  self->serial.Read (&self->serial, self->serialized_msg, sizeof(self->serialized_msg));
  //char *rec_msg = (char *) self->serialized_msg;
  self->velocity[left] = atoi(self->serialized_msg);
}


void fcSendMessage (Serialization *self) {
  char *ptr_to_left;
  char *ptr_to_right;

  ptr_to_left = (char *) &self->feedback_velocity[left];
  ptr_to_right = (char *) &self->feedback_velocity[right];

  for (i = 0; i < 2 * sizeof(float); i++){
    if(i < sizeof(float)){
      self->serialized_msg[i] = *(ptr_to_right + i);
    }
    else{
      self->serialized_msg[i] = *(ptr_to_left + i);
    }
  }

  self->serial.Write (&self->serial, self->serialized_msg);
}


void fcSetVelocity (Serialization *self, float *left_vel, float *right_vel) {
  self->velocity[left] = *left_vel;
  self->velocity[right] = *right_vel;
}


void fcGetVelocity (Serialization *self, float *left_fb_vel, float *right_fb_vel) {
  *left_fb_vel = self->feedback_velocity[left];
  *right_fb_vel = self->feedback_velocity[right];
}
