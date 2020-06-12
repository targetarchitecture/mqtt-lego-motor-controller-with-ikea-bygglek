#include <Arduino.h>
#include "common.h"

void Log(const String &payload)
 {
  if (WiFi.isConnected() == true)
  {
    if (MQTTClient.connected() == true)
    {
      MQTTClient.publish(MQTT_LOG_TOPIC, payload.c_str());
    }
  }

  Serial.println(payload);
}

void Log(const char *topic, const char *payload)
{
  if (WiFi.isConnected() == true)
  {
    if (MQTTClient.connected() == true)
    {
      MQTTClient.publish(topic, payload);
    }
  }

  Serial.println(payload);
}

void Log(const char *payload)
{
  if (WiFi.isConnected() == true)
  {
    if (MQTTClient.connected() == true)
    {
      MQTTClient.publish(MQTT_LOG_TOPIC, payload);
    }
  }

  Serial.println(payload);
}

void Log(String topic, String payload)
{
  if (WiFi.isConnected() == true)
  {
    if (MQTTClient.connected() == true)
    {
      MQTTClient.publish(topic.c_str(), payload.c_str());
    }
  }

  Serial.println(payload);
}

void setupWifi()
{
  //sort out WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to the network

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Log("Connection Failed!");
  }

  //make code smarter if it's not on the network it should still work
  if (WiFi.isConnected() == true)
  {
    Log("Ready on the local network");
    Log("IP address: " + WiFi.localIP().toString());

    if (!MDNS.begin(MDNS_HOSTNAME))
    { // Start the mDNS responder for .local
      Log("Error setting up MDNS responder!");
    }
    Log("mDNS responder started");
  }
}

void setupOTA()
{
  //make code smarter if it's not on the network it should still work
  if (WiFi.isConnected() == true)
  {
    ArduinoOTA.setHostname(MDNS_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });

    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
    });
    ArduinoOTA.begin();
  }
}
