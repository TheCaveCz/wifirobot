
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

  m1targetSpeed = m1;
  m2targetSpeed = m2;
}
