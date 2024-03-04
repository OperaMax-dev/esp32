#include "ConfigManager.h"
#include <EEPROM.h>

#define EEPROM_SIZE 512

void ConfigManager::begin() {
  EEPROM.begin(EEPROM_SIZE);
  updateInterval = EEPROM.readUInt(0);
  if (updateInterval == 0 || updateInterval == 0xFFFFFFFF) updateInterval = 5000;
  EEPROM.end();
}

const char* ConfigManager::getSSID() {
  return ssid;
}

const char* ConfigManager::getPassword() {
  return password;
}

int ConfigManager::getUpdateInterval() {
  return updateInterval;
}

void ConfigManager::save() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.writeUInt(0, updateInterval);
  EEPROM.commit();
  EEPROM.end();
}
