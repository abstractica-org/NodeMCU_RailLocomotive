#include <Arduino.h>
#include <WiFiManager.h>
#include "Locomotive.h"

#define WIFI_RESET_PIN D1

Locomotive locomotive;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println();
  Serial.println("Locomotive");
  pinMode(WIFI_RESET_PIN, INPUT_PULLUP);
  delay(10);
  bool resetWiFi = !digitalRead(WIFI_RESET_PIN);
  Serial.print("WiFiConfig -> Reset WiFi button pushed? : ");
  if(resetWiFi)
  {
    Serial.println("YES");
  }
  else
  {
    Serial.println("NO");
  }

  String apName = "Locomotive_";
  apName += ESP.getChipId();

  //WiFiManager
  WiFiManager wifiManager;
  //reset saved settings
  if(resetWiFi)
  {
    wifiManager.resetSettings();
  }
  
  wifiManager.autoConnect(apName.c_str());
  //if you get here you have connected to the WiFi
  Serial.println("WiFi is connected!");
  digitalWrite(LED_BUILTIN, HIGH);
  locomotive.begin(3377, 3377);
}

void loop()
{
  unsigned long curTime = millis();
  locomotive.update(curTime);
}