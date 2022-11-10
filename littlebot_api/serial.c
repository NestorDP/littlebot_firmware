#include "littlebot_api/serial.h"

void SerialInterfaceContruct(SerialInterface *self, uint32_t baud_rate) {
  BluetoothConfigure(baud_rate);
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
}

void fcReceiveMessage(SerialInterface *self, char *msg, uint32_t len_msg) {
  BluetoothRead(msg, len_msg);
}

void fcSendMessage(SerialInterface *self, char *msg) {
  BluetoothWrite(msg);
}
