#include "SecurityManager.h"
#include <ArduinoJWT.h>

bool SecurityManager::authenticateRequest(AsyncWebServerRequest *request) {
    if (request->hasHeader("Authorization")) {
        String auth = request->header("Authorization");
        if (auth.startsWith("Bearer ")) {
            String token = auth.substring(7);
            return validateJWT(token);
        }
    }
    return request->authenticate(username, password);
}

String SecurityManager::generateJWT(const char* username) {
    ArduinoJWT jwt(jwt_secret);
    String payload = "{\"user\":\"" + String(username) + "\",\"exp\":" + String(millis() + 3600000) + "}";
    return jwt.encode(payload);
}

bool SecurityManager::validateJWT(const String& token) {
    ArduinoJWT jwt(jwt_secret);
    String payload = jwt.decode(token);
    return payload.length() > 0;
}
