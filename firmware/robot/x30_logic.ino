uint8_t logicR;
uint8_t logicG;
uint8_t logicB;
uint8_t logicOn;

void logicStop() {
  logicSetSpeed(MOTORS_MIDDLE, MOTORS_MIDDLE);
}

void logicSetSpeed(const int sx, const int sy) {
  int p = sx;
  int q = sy - MOTORS_MIDDLE;
  int m1 = constrain(p + q, 0, MOTORS_MAX);
  int m2 = constrain(p - q, 0, MOTORS_MAX);

  if (m1targetSpeed != m1 || m2targetSpeed != m2) {
    logHeader();
    logRaw("M1 speed: ");
    logRaw(m1)
    logRaw(", M2 speed: ");
    logRaw(m2);
    logLine();
  }

  if (Blynk.connected() && (m1targetSpeed != m1 || m2targetSpeed != m2)) {
    Blynk.virtualWrite(BLYNK_M1_SPEED_PIN, m1);
    Blynk.virtualWrite(BLYNK_M2_SPEED_PIN, m2);
  }
  m1targetSpeed = m1;
  m2targetSpeed = m2;
}

void logicSetLight(const uint8_t r, const uint8_t g, const uint8_t b) {
  logValue("Set R: ", r);
  logValue("Set G: ", g);
  logValue("Set B: ", b);
  logicR = r;
  logicG = g;
  logicB = b;
  logicUpdateLight();
}

void logicSetLightOn(const uint8_t on) {
  logicOn = on ? 1 : 0;
  logValue("Set light on: ", logicOn);
  logicUpdateLight();
}

void logicUpdateLight() {
  if (logicOn) {
    pixelsSet(logicR, logicG, logicB);
  } else {
    pixelsSet(0, 0, 0);
  }
}

void logicSetup() {
  logicR = 128;
  logicG = 128;
  logicB = 128;
  logicOn = 1;
  logicUpdateLight();
}
