#include "littlebot_firmware/serialization.h"


void SerializationConstruct(Serialization *self) {
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;

  SerialInterfaceContruct(&self->serial, 115200);
}


void fcReceiveMessage (Serialization *self, float *left_vel, float *right_vel) {
  char serialized_msg[200];
  char var_left[4];
  char var_right[4];

  self->serial.Read (&self->serial, serialized_msg, sizeof(serialized_msg));

  strncpy (var_left, serialized_msg, 4);
  strncpy (var_right, serialized_msg + 4, 4);

  left_vel = (float *)var_left;
  right_vel = (float *)var_right;
}


void fcSendMessage (Serialization *self, float *left_vel, float *right_vel) {
  char serialized_msg[200];
  char *ptr_to_left;
  char *ptr_to_right;

  ptr_to_left = (char *) left_vel;
  ptr_to_right = (char *) right_vel;

  for (i = 0; i < 2 * 4; i++){
    if(i < 4){
      serialized_msg[i] = *(ptr_to_right + i);
    }
    else{
      serialized_msg[i] = *(ptr_to_left + i);
    }
  }
  
  self->serial.Write (&self->serial, serialized_msg);
}
