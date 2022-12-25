#include "littlebot_firmware/serialization.h"

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
  char var_left[sizeof(float)];
  char var_right[sizeof(float)];

  self->serial.Read (&self->serial, self->serialized_msg, sizeof(self->serialized_msg));

  strncpy (var_left, self->serialized_msg, 4);
  strncpy (var_right, self->serialized_msg + 4, 4);

  self->velocity[left] = atoi(var_left);
  self->velocity[right] = atoi(var_right);
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
  
  self->serial.Write (&self->serial, "self->serialized_msg");
}


void fcSetVelocity (Serialization *self, float *left_vel, float *right_vel) {
  self->velocity[left] = *left_vel;
  self->velocity[right] = *right_vel;
}


void fcGetVelocity (Serialization *self, float *left_fb_vel, float *right_fb_vel) {
  *left_fb_vel = self->feedback_velocity[left];
  *right_fb_vel = self->feedback_velocity[right];
}
