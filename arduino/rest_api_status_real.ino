
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Assume external sensor libraries are used
#include "LD2450.h"
#include "LD2412.h"
#include "SCD40.h"

LD2450Sensor ld2450;
LD2412Sensor ld2412;
SCD40Sensor scd40;

AsyncWebServer server(80);

int updateInterval = 5000;
String mqttBroker = "192.168.1.100";

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  ld2450.begin();
  ld2412.begin();
  scd40.begin();

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["co2"] = scd40.getCO2();
    doc["temp"] = scd40.getTemperature();
    doc["humidity"] = scd40.getHumidity();
    doc["ld2450"] = ld2450.targetsAvailable() ? "Active" : "None";
    doc["ld2412"] = ld2412.targetsAvailable() ? "Active" : "None";
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{"status":"ok"}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    updateInterval = doc["interval"] | 5000;
    mqttBroker = String((const char*)doc["mqtt"]);
  });

  server.begin();
}

void loop() {
  delay(updateInterval);
}
