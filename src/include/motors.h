
#ifndef motors_h

#define motors_h

#include <Arduino.h>
#include "credentials.h"
#include "LOLIN_I2C_MOTOR.h"

extern void Log(const String &payload);
extern void Log(const char *payload);
extern void Log(const char *topic, const char *payload);
extern void Log(String topic, String payload);

struct MotorXY
{
  int motor_x;
  int motor_y;
  bool fromMQTT;
};

class Motors
{
public:
  Motors();
  void Begin();
  void setMapped(int mapx, int mapy, int laserRangeMilliMeter); //, int medianCompassHeading);

private:
  LOLIN_I2C_MOTOR leftMotors;  //using customize I2C address
  LOLIN_I2C_MOTOR rightMotors; //I2C address 0x30
 //bool autoCorrectWithCompass = false;
  //int compassHeadingWhenStartedLinear = -1;
};

#endif