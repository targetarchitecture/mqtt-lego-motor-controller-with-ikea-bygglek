#ifndef Nunchuck_h

#define Nunchuck_h

#include <Arduino.h>
#include "credentials.h"
#include "motors.h"

extern void Log(const String &payload);
extern void Log(const char *payload);
extern void Log(const char *topic, const char *payload);
extern void Log(String topic, String payload);

class Nunchuck
{
public:
  Nunchuck();
  void nunchuck_init();
  MotorXY Loop();

private:
  byte accx, accy, zbut, cbut, joyx, joyy;
  void nunchuck_send_request();
  char nunchuk_decode_byte(char x);
  int nunchuck_get_data();
  void nunchuck_print_data();
  int nunchuck_zbutton();
  int nunchuck_cbutton();
  int nunchuck_joyx();
  int nunchuck_joyy();
  int nunchuck_accelx();
  int nunchuck_accely();
  int nunchuck_accelz();
  uint8_t nunchuck_buf[6]; // array to store nunchuck data,
};

#endif