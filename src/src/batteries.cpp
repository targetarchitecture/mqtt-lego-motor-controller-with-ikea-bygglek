#include <Arduino.h>
#include "batteries.h"

ADC_MODE(ADC_VCC);

Battery::Battery()
{
}

void Battery::Begin()
{
 //http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/reference.html#analog-input
  String msg = "Battery VCC:" + String((float)ESP.getVcc() / 1024.0) + "v";

  // publish laser distance to topic
  Log(MQTT_BATTERY_TOPIC, msg);
}

