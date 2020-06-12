#ifndef Compass_h

#define Compass_h

#include <Arduino.h>
#include "credentials.h"
#include <MedianFilter.h> // https://github.com/daPhoosa/MedianFilter
#include "QMC5883L.h"     // https://github.com/dthain/QMC5883L

extern void Log(const String &payload);
extern void Log(const char *payload);
extern void Log(const char *topic, const char *payload);
extern void Log(String topic, String payload);

class Compass
{
public:
  Compass();
  void Begin();
  int Loop();

private:
  QMC5883L sensor;
  MedianFilter medianCompassHeadings;
};

#endif