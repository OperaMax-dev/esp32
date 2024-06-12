#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include <ESPAsyncWebServer.h>

class SecurityManager {
private:
    const char* username;
    const char* password;
    const char* jwt_secret;

public:
    SecurityManager(const char* username, const char* password, const char* jwt_secret)
        : username(username), password(password), jwt_secret(jwt_secret) {}
    
    bool authenticateRequest(AsyncWebServerRequest *request);
    String generateJWT(const char* username);
    bool validateJWT(const String& token);
};

#endif
