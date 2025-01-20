#include <Arduino.h>
#include <Train.h>
#include <WiFiManager.h>

#define MOTOR_PWM_PIN 13  //D7 
#define MOTOR_DIR_PIN_A 15  //D8 Ora
#define MOTOR_DIR_PIN_B 0 //D3
#define WIFI_RESET_PIN 4 //D2
#define FRONT_SENSOR_PIN 4 //D2
#define BACK_SENSOR_PIN 5 //D1
#define LEFT_SENSOR_PIN 12 //D6
#define RIGHT_SENSOR_PIN 14 //D5

Train train(MOTOR_PWM_PIN, MOTOR_DIR_PIN_A, MOTOR_DIR_PIN_B, 20, FRONT_SENSOR_PIN, BACK_SENSOR_PIN, LEFT_SENSOR_PIN, RIGHT_SENSOR_PIN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
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

  train.begin(3377, 3377);
}

void loop()
{
  train.update(millis());
}