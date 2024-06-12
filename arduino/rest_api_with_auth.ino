#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* http_username = "admin";
const char* http_password = "esp32";

AsyncWebServer server(80);

bool isAuthenticated(AsyncWebServerRequest *request) {
  return request->authenticate(http_username, http_password);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  server.on("/logs/latest", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    request->send(200, "text/plain", "Simulated log content");
  });

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    request->send(200, "application/json", "{"status": "ok"}");
  });

  server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!isAuthenticated(request)) return request->requestAuthentication();
    request->send(200, "application/json", "{"saved": true}");
  });

  server.begin();
}

void loop() {}
