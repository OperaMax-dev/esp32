#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "esp_task_wdt.h"

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  File logFile = LittleFS.open("/log.txt", "w");
  logFile.println("Log Start");
  logFile.println("LD2450 initialized");
  logFile.println("CO2 level: 419 ppm");
  logFile.close();

  server.on("/logs/latest", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/log.txt", "text/plain");
  });

  server.on("/fault/test", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Triggering fault for watchdog...");
    delay(100);
    while (true);  // Simulate hang to trigger WDT
  });

  esp_task_wdt_init(3, true); // 3s timeout
  esp_task_wdt_add(NULL);
  server.begin();
}

void loop() {
  esp_task_wdt_reset();
  delay(1000);
}
