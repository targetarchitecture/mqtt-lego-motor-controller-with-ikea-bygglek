#ifndef Common_h

#define Common_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <PubSubClient.h>
#include "credentials.h"

extern PubSubClient MQTTClient;

void setupWifi();
void setupOTA();
void Log(const String &payload);
void Log(const char *payload);
void Log(const char *topic, const char *payload);
void Log(String topic, String payload);

#endif