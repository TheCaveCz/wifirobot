
uint32_t blynkConnectionCheckTime;


BLYNK_CONNECTED() {
  logInfo("Blynk connected");

  Blynk.syncAll();
  pixelsSetAnimState(PIXELS_ANIM_NORMAL);
  pixelsSet(0, 0, 0);
}

BLYNK_DISCONNECTED() {
  logInfo("Blynk disconnected");
  pixelsSetAnimState(PIXELS_ANIM_RED);
  logicStop();
}

BLYNK_WRITE(BLYNK_JOYSTICK_PIN) {
  logicSetSpeed(param[0].asInt(), param[1].asInt());
}

BLYNK_WRITE(BLYNK_LIGHT_PIN) {
  logicSetLightOn(param.asInt());
}

BLYNK_WRITE(BLYNK_RGB_PIN) {
  logicSetLight(param[0].asInt(), param[1].asInt(), param[2].asInt());
}

bool blynkIsConfigured() {
  return strlen(config.blynkToken) != 0;
}

void blynkConnectIfPossible() {
  if (WiFi.isConnected() && !Blynk.connected()) {
    logInfo("Blynk connect attempt");
    pixelsSetAnimState(PIXELS_ANIM_GREEN);
    Blynk.connect();
  }
}

void blynkTick() {
  if (!blynkIsConfigured()) return;

  if (millis() > blynkConnectionCheckTime) {
    logInfo("Blynk connection check");
    blynkConnectionCheckTime = millis() + RECONNECT_INTERVAL;
    blynkConnectIfPossible();
  }

  if (Blynk.connected()) {
    Blynk.run();
  }
}

void blynkSetup() {
  Blynk.config(config.blynkToken);
  blynkConnectionCheckTime = 0;
}
