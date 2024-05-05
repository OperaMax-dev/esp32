#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/api/command", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{"status":"received"}");
  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, data);
    if (!err) {
      const char* cmd = doc["cmd"];
      if (strcmp(cmd, "toggle") == 0) {
        digitalWrite(2, !digitalRead(2));
      }
    }
  });

  pinMode(2, OUTPUT);
  server.begin();
}

void loop() {}
