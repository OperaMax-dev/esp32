#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "controller.h"
#include "model.h"
#include "view.h"

AsyncWebServer server(80);

void setup() {
  WiFi.begin("SSID", "PASSWORD");
  pinMode(2, OUTPUT);
  server.on("/api/command", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", "{"status":"ok"}");
  });
  server.begin();
}

void loop() {}
