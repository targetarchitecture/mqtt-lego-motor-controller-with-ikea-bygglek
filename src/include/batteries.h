
#ifndef Batteries_h

#define Batteries_h

#include <Arduino.h>
#include "credentials.h"

extern void Log(const String &payload);
extern void Log(const char *payload);
extern void Log(const char *topic, const char *payload);
extern void Log(String topic, String payload);

class Battery
{
public:
  Battery();
  void Begin();

private:
};

#endif