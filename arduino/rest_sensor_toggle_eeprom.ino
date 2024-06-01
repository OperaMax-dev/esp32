#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

#define EEPROM_SIZE 512

AsyncWebServer server(80);

bool ld2450_enabled = true;
bool ld2412_enabled = true;

void loadSensorStates() {
  EEPROM.begin(EEPROM_SIZE);
  ld2450_enabled = EEPROM.read(100) != 0;
  ld2412_enabled = EEPROM.read(101) != 0;
  EEPROM.end();
}

void saveSensorStates() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.write(100, ld2450_enabled ? 1 : 0);
  EEPROM.write(101, ld2412_enabled ? 1 : 0);
  EEPROM.commit();
  EEPROM.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  loadSensorStates();

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["ld2450"] = ld2450_enabled ? "Active" : "Disabled";
    doc["ld2412"] = ld2412_enabled ? "Active" : "Disabled";
    doc["co2"] = 421;
    doc["temp"] = 23.1;
    doc["humidity"] = 41.5;
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/api/sensors", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{"status":"updated"}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    if (doc.containsKey("ld2450")) ld2450_enabled = doc["ld2450"];
    if (doc.containsKey("ld2412")) ld2412_enabled = doc["ld2412"];
    saveSensorStates();
  });

  server.begin();
}

void loop() {
  delay(1000);
}