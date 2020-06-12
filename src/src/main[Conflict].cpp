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
// void setupWifi();
// void setupOTA();
// void Log(const String &payload);
// void Log(const char *payload);
// void Log(const char *topic, const char *payload);
// void Log(String topic, String payload);

PubSubClient MQTTClient;
MQTT mqtt;
Laser laser;
Battery battery;
Compass compass;
Motors motors;
Nunchuck nunchuck;

void setup()
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("Starting");

  setupWifi();
  setupOTA();

  // MQTTClient.publish(MQTT_LOG_TOPIC, (String("Connected to SSID: ") + String(WIFI_SSID)).c_str());
  // MQTTClient.publish(MQTT_LOG_TOPIC, (String("IP address: ") + WiFi.localIP().toString());
  //MQTTClient.publish(MQTT_LOG_TOPIC, (String("mDNS: ") + String(MDNS_HOSTNAME)).c_str());
  //MQTTClient.publish(MQTT_LOG_TOPIC, (String("Connected to MQTT server: ") + String(MQTT_SERVER)).c_str();

  delay(500);

  i2c_scanner();
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

  motors.setMapped(motor_x, motor_y, laserRangeMilliMeter, medianCompassHeading);

  //read_MQTT = false;

  delay(50);
}


void i2c_scanner()
{
  yield();

  byte error, address;
  int nDevices;

  // MQTTClient.publish(MQTT_LOG_TOPIC, "Scanning");

  Log("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    yield();

    delay(5);

    // The i2c scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
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
  }
  else
  {
    Log("Done.\n");
  }
}
