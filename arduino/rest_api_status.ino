#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer server(80);

float mockCO2 = 420.0;
float mockTemp = 22.5;
float mockHumidity = 45.0;
int mockLD2450 = 1;
int mockLD2412 = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    StaticJsonDocument<256> doc;
    doc["co2"] = mockCO2;
    doc["temp"] = mockTemp;
    doc["humidity"] = mockHumidity;
    doc["ld2450"] = mockLD2450 ? "Active" : "None";
    doc["ld2412"] = mockLD2412 ? "Active" : "None";

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });

  server.begin();
}

void loop() {}