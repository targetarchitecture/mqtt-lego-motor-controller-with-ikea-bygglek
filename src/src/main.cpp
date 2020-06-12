#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <PubSubClient.h>
#include "credentials.h"
#include "common.h"
#include "mqttClient.h"
#include "compass.h"
#include "laser.h"
#include "motors.h"
#include "batteries.h"
#include "nunchuck.h"

void i2c_scanner();

PubSubClient MQTTClient;
MQTT mqtt;
Battery battery;
Motors motors;
Nunchuck nunchuck;
Laser laser;
Compass compass;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting");

  Wire.begin();

  setupWifi();
  setupOTA();

  //start MQTT
  mqtt.Begin();

  i2c_scanner();

  //start laser beam
  laser.Begin();

  //start compass
  compass.Begin();

  //get battery reading
  battery.Begin();

  //get nunchuck ready
  nunchuck.nunchuck_init();

  //get motors ready
  motors.Begin();
}

void loop()
{
  //make code smarter if it's not on the network it should still work
  if (WiFi.isConnected() == true)
  {
    MDNS.update();
    ArduinoOTA.handle();
  }

  MotorXY motorXY;
  motorXY = mqtt.Loop();

  if (motorXY.fromMQTT == false)
  {
    motorXY = nunchuck.Loop();
  }

  //go and get laser and compass values
  int laserRangeMilliMeter = laser.Loop();
  int medianCompassHeading = compass.Loop();
  int motor_x = motorXY.motor_x;
  int motor_y = motorXY.motor_y;

  motors.setMapped(motor_x, motor_y, laserRangeMilliMeter); //, medianCompassHeading);

  delay(50);
}

void i2c_scanner()
{
  yield();

  byte error, address;
  int nDevices;

  Log("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    yield();

    delay(5);

    // The i2c scanner uses the return value of the Write.endTransmisstion to see if a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Log("I2C device found at address 0x");
      if (address < 16)
      {
        Log("0");
      }
      Log(String(address));
      Log(" !");

      nDevices++;
    }
    else if (error == 4)
    {
      Log("Unknown error at address 0x");
      if (address < 16)
      {
        Log("0");
      }
      Log(String(address));
    }
  }
  if (nDevices == 0)
  {
    Log("No I2C devices found\n");

    delay(500);

    ESP.restart();
  }
  else
  {
    Log("Done.\n");
  }
}
