
void setup() {
  Serial.begin(115200);

  logInfo("Starting");
  logValue("Chip id: ", chipId);

  pixelsSetup();
  configSetup();
  motorsSetup();
  adcSetup();
  otaSetup();
  wifiSetup();
  blynkSetup();
  logInfo("Ready to go!");

  timeoutRefresh();
}

void loop() {
  if (Blynk.connected()) {
    Blynk.run();
  }

  ArduinoOTA.handle();

  if (wifiGotIpFlag) {
    wifiGotIpFlag = false;

    if (blynkIsConfigured()) {
      // we have blynk token, try to connect there
      blynkConnectIfPossible();
    } else {
      timeoutClear();
      pixelsSetAnimState(PIXELS_ANIM_RED);
    }
  }

  if (timeoutCheck()) {
    pixelsSetAnimState(PIXELS_ANIM_RED);
  }
  blynkCheck();
  adcTick();
  motorsTick();
}
