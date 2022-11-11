#include "littlebot_api/serialization.h"

void SerializationInterfaceConstruct(SerializationInterface *self) {
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
  self->SetVelocity = fcSetVelocity;
  self->GetVelocity = fcGetVelocity;
}

void fcReceiveMessage(SerializationInterface *self, char *msg) {
  
}

void fcSendMessage(SerializationInterface *self, char *msg) {
  
}

void fcSetVelocity (SerializationInterface *self, float vel) {

}

float fcGetVelocity (SerializationInterface *self) {
  
}
