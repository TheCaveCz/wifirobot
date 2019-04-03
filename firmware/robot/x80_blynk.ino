
bool blynkFirstConnect;
uint32_t blynkConnectionCheckTime;


BLYNK_CONNECTED() {
  logInfo("Blynk connected");

  timeoutClear();
  Blynk.syncAll();
  pixelsSetAnimState(PIXELS_ANIM_NORMAL);
  pixelsSet(32, 32, 32);
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
  if (param.asInt()) {
    pixelsSet(128, 128, 128);
  } else {
    pixelsSet(1, 1, 1);
  }
}

bool blynkIsConfigured() {
  return strlen(config.blynkToken) != 0;
}

void blynkConnectIfPossible() {
  if (WiFi.isConnected() && !Blynk.connected()) {
    logInfo("Blynk connect attempt");
    pixelsSetAnimState(PIXELS_ANIM_GREEN);
    timeoutRefresh();
    Blynk.connect();
  }
}

void blynkCheck() {
  if (!timeoutIsActive() && blynkConnectionCheckTime && millis() > blynkConnectionCheckTime) {
    logInfo("Blynk connection check");
    blynkConnectionCheckTime = millis() + RECONNECT_INTERVAL;
    blynkConnectIfPossible();
  }
}

void blynkSetup() {
  Blynk.config(config.blynkToken);
  blynkConnectionCheckTime = blynkIsConfigured() ? millis() + RECONNECT_INTERVAL : 0;
}
