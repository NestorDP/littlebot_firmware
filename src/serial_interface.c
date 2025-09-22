#include "littlebot_firmware/serial_interface.h"

void SerialInterfaceConstructor(SerialInterface *self, uint32_t baud_rate) {
  UartPortConfigure(baud_rate);
  self->Read = fcRead;
  self->Write = fcWrite;
}

uint32_t fcRead(SerialInterface *self, char *msg) {
  return UartPortRead(msg);
}

void fcWrite(SerialInterface *self, char *msg) {
  UartPortWrite(msg, strlen(msg));
}
