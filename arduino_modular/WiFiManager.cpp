#include "WiFiManager.h"

bool WiFiManager::begin() {
    WiFi.begin(ssid, password);
    unsigned long startAttempt = millis();
    
    while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
        delay(500);
    }
    
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::loop() {
    if (!isConnected() && (millis() - lastReconnectAttempt >= reconnectInterval)) {
        reconnect();
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnect() {
    lastReconnectAttempt = millis();
    WiFi.disconnect();
    delay(1000);
    WiFi.begin(ssid, password);
}
