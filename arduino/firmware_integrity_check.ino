#include <Update.h>

void setup() {
  Serial.begin(115200);
  uint32_t crc = 0;
  for (uint32_t i = 0x10000; i < ESP.getSketchSize(); i++) {
    crc ^= *(uint8_t*)(i);
  }
  Serial.printf("Firmware CRC32: 0x%08X\n", crc);
}

void loop() {}
