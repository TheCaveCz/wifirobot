/*
 * WifiRobot firmware for Blynk app
 * 
 * The Cave, 2019
 * https://thecave.cz
 * 
 * Licensed under MIT License (see LICENSE file for details)
 * 
 * board: LOLIN(WEMOS) D1 R2 & mini
 * 
 */
#define BLYNK_PRINT Serial
#define BLYNK_NO_FANCY_LOGO

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "ws2812.h"


// Firmware updates over-the-air will require admin password.
//#define OTA_PASSWORD "mySecretPassword"

// Serial port logging
#define LOG_ENABLED 1


#define BLYNK_JOYSTICK_PIN V0
#define BLYNK_LIGHT_PIN V1
#define BLYNK_RGB_PIN V2
#define BLYNK_BATTERY_PERCENT_PIN V3
#define BLYNK_BATTERY_VOLTAGE_PIN V4
#define BLYNK_M1_SPEED_PIN V5
#define BLYNK_M2_SPEED_PIN V6
#define BLYNK_BRAKE_PIN V7


#define PIN_M1_A D7
#define PIN_M1_B D0
#define PIN_M1_EN D5

#define PIN_M2_A D3
#define PIN_M2_B D6
#define PIN_M2_EN D8

#define HOSTNAME_PREFIX "robot-"

#define TIMEOUT_INTERVAL 20000UL
#define RECONNECT_INTERVAL 60000UL

#define UDP_PORT 44432

String chipId = String(ESP.getChipId(),HEX);

void keep_me_here() {} // must be present for Arduino IDE function prototype injection
