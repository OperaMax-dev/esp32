#include "WebServerSetup.h"
#include <ArduinoJson.h>
#include <LittleFS.h>

void setupWebServer(AsyncWebServer& server, SensorManager& sensors, ConfigManager& config) {
  LittleFS.begin();

  server.on("/api/status", HTTP_GET, [&](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["ld2450"] = sensors.ld2450_enabled ? "Active" : "Disabled";
    doc["ld2412"] = sensors.ld2412_enabled ? "Active" : "Disabled";
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/logs/latest", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/log.txt", "text/plain");
  });
}
