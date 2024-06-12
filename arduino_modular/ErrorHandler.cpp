#include "ErrorHandler.h"
#include <LittleFS.h>

void ErrorHandler::begin() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }
}

void ErrorHandler::logError(const char* module, const char* error) {
    logFile = LittleFS.open(logPath, "a");
    if (logFile) {
        logFile.printf("[%lu] %s: %s\n", millis(), module, error);
        logFile.close();
    }
}

void ErrorHandler::logSensorError(const char* sensorName, const char* error) {
    logError(sensorName, error);
    // Additional sensor-specific error handling could be added here
}
