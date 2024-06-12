#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <FS.h>

class ErrorHandler {
private:
    File logFile;
    const char* logPath = "/error.log";

public:
    void begin();
    void logError(const char* module, const char* error);
    void logSensorError(const char* sensorName, const char* error);
};

#endif
