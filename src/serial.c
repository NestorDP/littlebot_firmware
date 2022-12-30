#include "littlebot_firmware/serial.h"

void SerialInterfaceContruct(SerialInterface *self, uint32_t baud_rate) {
  UartPortConfigure(baud_rate);
  self->Read = fcRead;
  self->Write = fcWrite;
}

uint32_t fcRead(SerialInterface *self, char *msg) {
  return UartPortGet(msg);
}

void fcWrite(SerialInterface *self, char *msg) {
  UartPortPut(msg);
}
