#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

class WiFiManager {
private:
    const char* ssid;
    const char* password;
    unsigned long lastReconnectAttempt = 0;
    const unsigned long reconnectInterval = 30000; // 30 seconds

public:
    WiFiManager(const char* ssid, const char* password) : ssid(ssid), password(password) {}
    
    bool begin();
    void loop();
    bool isConnected();
    void reconnect();
};

#endif
