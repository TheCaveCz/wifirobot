/*
   WifiRobot test firmware for Blynk app

   The Cave, 2019
   https://thecave.cz

   Licensed under MIT License (see LICENSE file for details)

   board: LOLIN(WEMOS) D1 R2 & mini

*/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ws2812.h"

#define WIFI_SSID "TheCave"
#define WIFI_PASS "enter your password"

#define PIN_M1_A D7
#define PIN_M1_B D0
#define PIN_M1_EN D5

#define PIN_M2_A D3
#define PIN_M2_B D6
#define PIN_M2_EN D8

#define MOTORS_MAX 255
#define MOTORS_MIDDLE 127

#define ADC_BUTTON_TRESHOLD 20


void pixelsSet(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t pixelsBuffer[3];

  pixelsBuffer[0] = r;
  pixelsBuffer[1] = g;
  pixelsBuffer[2] = b;

  ws2812Send(pixelsBuffer, 3);
  delay(2);
}

void wifiConnect() {
  Serial.println("Connecting");
  pixelsSet(0, 0, 255);

  if (WiFi.getMode() != WIFI_STA) WiFi.mode(WIFI_STA);
  if (WiFi.getAutoConnect()) WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    pixelsSet(0, 0, 32);
    delay(250);
    pixelsSet(0, 0, 128);
    delay(250);
    counter++;
    if (counter > 30) {
      Serial.println("\nConnect timed out");
      break;
    }
  }

  Serial.print("\nConnect status: ");
  Serial.println(WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected successfuly\nIP address: ");
    Serial.println(WiFi.localIP());
  } else {
    pixelsSet(255, 0, 0);
    while (1) yield();
  }
}

uint16_t motorsSpeedToPwm(const uint16_t speed) {
  if (speed == MOTORS_MIDDLE) {
    return 0;
  } else if (speed > MOTORS_MIDDLE) {
    return map(speed, MOTORS_MIDDLE, MOTORS_MAX, PWMRANGE / 2, PWMRANGE);
  } else {
    return map(speed, MOTORS_MIDDLE - 1, 0, PWMRANGE / 2, PWMRANGE);
  }
}

void motorsSet(const uint8_t m1speed, const uint8_t m2speed) {
  digitalWrite(PIN_M1_A, m1speed > MOTORS_MIDDLE);
  digitalWrite(PIN_M1_B, m1speed < MOTORS_MIDDLE);
  analogWrite(PIN_M1_EN, motorsSpeedToPwm(m1speed));

  digitalWrite(PIN_M2_A, m2speed > MOTORS_MIDDLE);
  digitalWrite(PIN_M2_B, m2speed < MOTORS_MIDDLE);
  analogWrite(PIN_M2_EN, motorsSpeedToPwm(m2speed));
}

void setup() {
  analogWriteFreq(20000);

  pinMode(PIN_M1_A, OUTPUT);
  pinMode(PIN_M1_B, OUTPUT);
  pinMode(PIN_M1_EN, OUTPUT);

  pinMode(PIN_M2_A, OUTPUT);
  pinMode(PIN_M2_B, OUTPUT);
  pinMode(PIN_M2_EN, OUTPUT);

  ws2812Setup();

  String chipId = String(ESP.getChipId(), HEX);

  Serial.begin(115200);
  Serial.println("Robot tester");
  Serial.print("Chip id: ");
  Serial.println(chipId);

  wifiConnect();

  String ssid = "robot-";
  ssid += chipId;
  ArduinoOTA.setHostname(ssid.c_str());

  ArduinoOTA.onStart([]() {
    Serial.println("OTA update start");
    motorsSet(MOTORS_MIDDLE, MOTORS_MIDDLE);
    pixelsSet(32, 32, 32);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA update end");
  });

  static uint32_t lastProgress = 0;
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    uint32_t p = map(progress, 0, total, 0, 100);
    if (p != lastProgress) {
      Serial.printf("Progress: %u %%\n", p);
      lastProgress = p;
    }
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  motorsSet(MOTORS_MIDDLE, MOTORS_MIDDLE);
  pixelsSet(255, 128, 0);
}

bool buttonRead() {
  int v = analogRead(A0);
  return v < ADC_BUTTON_TRESHOLD;
}

bool lastButton = 0;

void loop() {
  bool b = buttonRead();
  if (b != lastButton) {
    lastButton = b;
    if (b) {
      motorsSet(MOTORS_MAX, 0);
      pixelsSet(0, 255, 0);
    } else {
      motorsSet(MOTORS_MIDDLE, MOTORS_MIDDLE);
      pixelsSet(255, 128, 0);
    }
  }
  ArduinoOTA.handle();
}
