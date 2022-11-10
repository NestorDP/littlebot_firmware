#include "littlebot_api/serial.h"

void SerialInterfaceContruct(SerialInterface *self, uint32_t baud_rate) {
  UartPortConfigure(baud_rate);
  self->ReceiveMessage = fcReceiveMessage;
  self->SendMessage = fcSendMessage;
}

void fcReceiveMessage(SerialInterface *self, char *msg, uint32_t len_msg) {
  UartPortRead(msg, len_msg);
}

void fcSendMessage(SerialInterface *self, char *msg) {
  UartPortWrite(msg);
}
