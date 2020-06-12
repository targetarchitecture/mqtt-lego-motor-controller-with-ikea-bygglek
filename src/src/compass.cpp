#include <Arduino.h>
#include "compass.h"

Compass::Compass() : medianCompassHeadings(15, 0), sensor()
{
  Log("QMC5883L Compass");
}

void Compass::Begin()
{
  Log("QMC5883L Compass init");

  sensor.init();
  sensor.setSamplingRate(100);

  Log("Turn compass in all directions to calibrate....");

  //get an average to start with
  for (int i = 0; i <= 20; i++)
  {
    int compassHeading = sensor.readHeading();

    if (compassHeading == 0)
    {
      // publish compass details to topic
      Log(MQTT_COMPASS_TOPIC, "Still calibrating");
    }
    else
    {
      Log("Heading = " + String(compassHeading));
    }

    delay(10);
    yield();
  }
}

int Compass::Loop()
{
  int compassHeading = sensor.readHeading();

  if (compassHeading == 0)
  {    // publish compass details to topic
    Log(MQTT_COMPASS_TOPIC, "Still calibrating");
  }
  else
  {
    Log(MQTT_COMPASS_HEADING_TOPIC, String(compassHeading));

    compassHeading = medianCompassHeadings.in(compassHeading);

    Log(MQTT_COMPASS_MEDIAN_TOPIC, String(compassHeading));
  }

  return compassHeading;
}
