#include "littlebot_api/serialization.h"

void SerializationConstruct(Serialization *self) {
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
  self->encoder_velocity[left] = 4.7838322;
  self->encoder_velocity[right] = 2.4;
  self->motor_velocity[left] = 0;
  self->motor_velocity[right] = 0;
  SerialInterfaceContruct(&self->serial, 115200);
}


void fcReceiveMessage (Serialization *self) {
  self->serial.Read (&self->serial, self->serialized_msg, sizeof(self->serialized_msg));
  //char *rec_msg = (char *) self->serialized_msg;
  self->motor_velocity[left] = atoi(self->serialized_msg);
}


void fcSendMessage (Serialization *self) {
  ptr_to_float = (unsigned char *) &self->encoder_velocity[left];
  
  for (i = 0; i < sizeof(float); i++){
      self->serialized_msg[i] = *(ptr_to_float + i);
  }

  self->serial.Write (&self->serial, self->serialized_msg);
}


void fcSetVelocity (Serialization *self, float vel) {
  self->motor_velocity[0] = vel;
}


float fcGetVelocity (Serialization *self) {
  return self->encoder_velocity[0];
}


// SEND FLOAT VARIABLE OVER UART ALGORITHM
// int main()
// {
//     float f = 4.7838322;
//     unsigned char *ptr, i;
//     float *a;    
//     unsigned char var[sizeof(float)];
//     ptr = (unsigned char *)& f;
//     for (i = 0; i < sizeof(float); i++){
//         var[i] = *(ptr + i);
//     }
//     printf("Valores float dividido em bytes %s", var);
//     a = (float*) &var[0];
//     printf("Valores float para um ponteiro array char %f", *a);
//     return 0;
// }
