#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "SensorManager.h"
#include "ConfigManager.h"
#include "WebServerSetup.h"
#include "WiFiManager.h"
#include "SecurityManager.h"
#include "ErrorHandler.h"

SensorManager sensorManager;
ConfigManager configManager;
AsyncWebServer server(80);
WiFiManager wifiManager("SSID", "PASSWORD");
SecurityManager securityManager("admin", "esp32admin", "your-secret-key");
ErrorHandler errorHandler;

void setup() {
    Serial.begin(115200);
    
    errorHandler.begin();
    
    if (!wifiManager.begin()) {
        errorHandler.logError("WiFi", "Initial connection failed");
    }
    
    if (!LittleFS.begin()) {
        errorHandler.logError("FS", "LittleFS mount failed");
        return;
    }
    
    configManager.begin();
    
    if (!sensorManager.begin()) {
        errorHandler.logError("Sensors", "Initialization failed");
    }
    
    setupWebServer(server, sensorManager, configManager, securityManager);
    server.begin();
}

void loop() {
    wifiManager.loop();
    
    if (wifiManager.isConnected()) {
        sensorManager.loop();
    }
    
    delay(configManager.getUpdateInterval());
}
