#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer server(80);

bool ld2450_enabled = true;
bool ld2412_enabled = true;

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["ld2450"] = ld2450_enabled ? "Active" : "Disabled";
    doc["ld2412"] = ld2412_enabled ? "Active" : "Disabled";
    doc["co2"] = 419;
    doc["temp"] = 22.8;
    doc["humidity"] = 40.5;
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.on("/api/sensors", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{"updated":true}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, data);
    if (doc.containsKey("ld2450")) ld2450_enabled = doc["ld2450"];
    if (doc.containsKey("ld2412")) ld2412_enabled = doc["ld2412"];
  });

  server.begin();
}

void loop() {
  delay(1000);
}
