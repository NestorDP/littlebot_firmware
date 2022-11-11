#include "littlebot_api/serialization.h"

void SerializationConstruct(Serialization *self) {
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
  self->encoder_velocity = 0;
  self->motor_velocity = 0;
  SerialInterfaceContruct(&self->serial, 115200);
}

void fcReceiveMessage (Serialization *self) {
  self->serial.Read (&self->serial, self->serialized_msg, sizeof(self->serialized_msg));
}

void fcSendMessage (Serialization *self) {
  self->serial.Write (&self->serial, self->serialized_msg);
}

void fcSetVelocity (Serialization *self, float vel) {
  self->motor_velocity = vel;
}

float fcGetVelocity (Serialization *self) {
  return self->encoder_velocity;
}
