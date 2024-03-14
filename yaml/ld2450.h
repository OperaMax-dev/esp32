#ifndef LD2450_H
#define LD2450_H

class LD2450Sensor {
public:
  void begin();
  bool available();
  int read();
};

#endif
