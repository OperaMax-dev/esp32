#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SensorManager.h"
#include "ConfigManager.h"
#include "WebServerSetup.h"

SensorManager sensorManager;
ConfigManager configManager;
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(configManager.getSSID(), configManager.getPassword());
  while (WiFi.status() != WL_CONNECTED) delay(500);

  configManager.begin();
  sensorManager.begin();
  setupWebServer(server, sensorManager, configManager);
  server.begin();
}

void loop() {
  sensorManager.loop();
  delay(configManager.getUpdateInterval());
}
