#pragma once
#include <Arduino.h>

class ConfigManager {
public:
  void begin();
  const char* getSSID();
  const char* getPassword();
  int getUpdateInterval();
  void save();
private:
  const char* ssid = "SSID";
  const char* password = "PASSWORD";
  int updateInterval = 5000;
};
