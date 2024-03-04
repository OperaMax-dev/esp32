#pragma once
class SensorManager {
public:
  void begin();
  void loop();
  bool ld2450_enabled = true;
  bool ld2412_enabled = true;
};
