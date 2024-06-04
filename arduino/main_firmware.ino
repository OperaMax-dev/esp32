#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <FS.h>
#include <LittleFS.h>

#define EEPROM_SIZE 512
AsyncWebServer server(80);

// Configurable settings
int updateInterval = 5000;
String mqttBroker = "192.168.1.100";

// Sensor control flags
bool ld2450_enabled = true;
bool ld2412_enabled = true;

// WiFi credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Auth (optional)
const char* http_username = "admin";
const char* http_password = "esp32";

// Helpers
bool isAuthenticated(AsyncWebServerRequest *request) {
  return request->authenticate(http_username, http_password);
}

void loadSettings() {
  EEPROM.begin(EEPROM_SIZE);
  updateInterval = EEPROM.readUInt(0);
  if (updateInterval == 0xFFFFFFFF || updateInterval == 0) updateInterval = 5000;

  char mqttBuf[64];
  for (int i = 0; i < 64; ++i) mqttBuf[i] = EEPROM.read(4 + i);
  mqttBuf[63] = 0;
  mqttBroker = String(mqttBuf);

  ld2450_enabled = EEPROM.read(100) != 0;
  ld2412_enabled = EEPROM.read(101) != 0;
  EEPROM.end();
}

void saveSettings() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.writeUInt(0, updateInterval);
  for (int i = 0; i < mqttBroker.length() && i < 64; ++i) {
    EEPROM.write(4 + i, mqttBroker[i]);
  }
  EEPROM.write(100, ld2450_enabled ? 1 : 0);
  EEPROM.write(101, ld2412_enabled ? 1 : 0);
  EEPROM.commit();
  EEPROM.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  EEPROM.begin(EEPROM_SIZE);
  LittleFS.begin();
  loadSettings();

  File logFile = LittleFS.open("/log.txt", "a");
  logFile.println("=== Boot ===");
  logFile.close();

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    StaticJsonDocument<256> doc;
    doc["co2"] = 419;
    doc["temp"] = 22.5;
    doc["humidity"] = 40.1;
    doc["ld2450"] = ld2450_enabled ? "Active" : "Disabled";
    doc["ld2412"] = ld2412_enabled ? "Active" : "Disabled";
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    request->send(200, "application/json", "{"saved":true}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    updateInterval = doc["interval"] | 5000;
    mqttBroker = String((const char*)doc["mqtt"]);
    saveSettings();
  });

  server.on("/api/sensors", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    request->send(200, "application/json", "{"updated":true}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    if (doc.containsKey("ld2450")) ld2450_enabled = doc["ld2450"];
    if (doc.containsKey("ld2412")) ld2412_enabled = doc["ld2412"];
    saveSettings();
  });

  server.on("/logs/latest", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    request->send(LittleFS, "/log.txt", "text/plain");
  });

  server.on("/api/reboot", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Rebooting...");
    delay(100);
    ESP.restart();
  });

  server.on("/api/reset", HTTP_POST, [](AsyncWebServerRequest *request){
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0);
    EEPROM.commit();
    EEPROM.end();
    request->send(200, "text/plain", "Settings cleared, rebooting...");
    delay(100);
    ESP.restart();
  });

  server.begin();
}

void loop() {
  delay(updateInterval);
}