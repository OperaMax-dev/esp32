#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define EEPROM_SIZE 512

AsyncWebServer server(80);

int updateInterval = 5000;
String mqttBroker = "192.168.1.100";

void loadSettings() {
  EEPROM.begin(EEPROM_SIZE);
  updateInterval = EEPROM.readUInt(0);
  if (updateInterval == 0xFFFFFFFF || updateInterval == 0) updateInterval = 5000;

  char mqttBuf[64];
  for (int i = 0; i < 64; ++i) mqttBuf[i] = EEPROM.read(4 + i);
  mqttBuf[63] = 0;
  mqttBroker = String(mqttBuf);
  EEPROM.end();
}

void saveSettings() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.writeUInt(0, updateInterval);
  for (int i = 0; i < mqttBroker.length() && i < 64; ++i) {
    EEPROM.write(4 + i, mqttBroker[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  loadSettings();

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["co2"] = 420;
    doc["temp"] = 22.5;
    doc["humidity"] = 40.1;
    doc["ld2450"] = "Active";
    doc["ld2412"] = "None";
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
    saveSettings();
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
