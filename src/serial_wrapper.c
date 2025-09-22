#include "littlebot_firmware/serial_wrapper.h"

void SerialWrapperConstructor(SerialWrapper *self, uint32_t baud_rate) {
  UartPortConfigure(baud_rate);
  self->Read = fcRead;
  self->Write = fcWrite;
}

uint32_t fcRead(SerialWrapper *self, char *msg) {
  return UartPortRead(msg);
}

void fcWrite(SerialWrapper *self, char *msg) {
  UartPortWrite(msg, strlen(msg));
}
