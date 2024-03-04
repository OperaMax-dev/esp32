#pragma once
#include <ESPAsyncWebServer.h>
#include "SensorManager.h"
#include "ConfigManager.h"

void setupWebServer(AsyncWebServer& server, SensorManager& sensors, ConfigManager& config);
