#ifndef Laser_h

#define Laser_h

#include <Arduino.h>
#include <limits.h>
#include "Adafruit_VL53L0X.h"
#include "credentials.h"

extern void Log(const String &payload);
extern void Log(const char *payload);
extern void Log(const char *topic, const char *payload);
extern void Log(String topic, String payload);

class Laser
{
public:
  Laser();
  void Begin();
  int Loop();

private:
  Adafruit_VL53L0X lox;
};

#endif
