#include "SensorManager.h"
#include <Arduino.h>

void SensorManager::begin() {
  Serial.println("SensorManager initialized");
}

void SensorManager::loop() {
  if (ld2450_enabled) {
    Serial.println("LD2450 reading...");
  }
  if (ld2412_enabled) {
    Serial.println("LD2412 reading...");
  }
}
