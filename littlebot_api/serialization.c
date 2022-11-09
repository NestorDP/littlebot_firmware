#include "littlebot_api/serialization.h"

void SerializationInterfaceContruct(SerializationInterface *self, uint32_t baud_rate) {
  BluetoothConfigure(baud_rate);
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
}

void fcReceiveMessage(SerializationInterface *self, char *msg, uint32_t len_msg) {
  BluetoothRead(msg, len_msg);
}

void fcSendMessage(SerializationInterface *self, char *msg) {
  BluetoothWrite(msg);
}
