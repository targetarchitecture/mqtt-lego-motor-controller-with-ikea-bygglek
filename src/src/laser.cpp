#include <Arduino.h>
#include "laser.h"

Laser::Laser() : lox()
{
  Log("Load Laser");
}

void Laser::Begin()
{
  Log("Adafruit VL53L0X initialise");

  if (!lox.begin())
  {
    Log("Failed to boot VL53L0X");
    delay(1000);
    ESP.restart();
  }

  // power
  Log("VL53L0X ready");
}

int Laser::Loop()
{
  int laserRangeMilliMeter = INT_MAX;

  VL53L0X_RangingMeasurementData_t measure;

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4)
  { // phase failures have incorrect data
    laserRangeMilliMeter = measure.RangeMilliMeter;

    // publish laser distance to topic
    Log(MQTT_LASER_TOPIC, String(laserRangeMilliMeter).c_str());
  }
  else
  {
    laserRangeMilliMeter = INT_MAX;

    // publish laser distance to topic
    Log(MQTT_LASER_TOPIC, "out of range");
  }

  return laserRangeMilliMeter;
}
