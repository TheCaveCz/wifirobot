#define MOTORS_ACCEL_PER_TICK 60
#define MOTORS_TICK 20UL

#define MOTORS_MAX 255
#define MOTORS_MIDDLE 127

int16_t m1speed;
int16_t m2speed;
int16_t m1targetSpeed;
int16_t m2targetSpeed;

uint32_t motorsTime;

uint16_t motorsSpeedToPwm(uint16_t speed) {
  if (speed == MOTORS_MIDDLE) {
    return 0;
  } else if (speed > MOTORS_MIDDLE) {
    return map(speed, MOTORS_MIDDLE, MOTORS_MAX, PWMRANGE / 1.5, PWMRANGE);
  } else {
    return map(speed, MOTORS_MIDDLE - 1, 0, PWMRANGE / 1.5, PWMRANGE);
  }
}

void motorsTick() {
  if (millis() < motorsTime) {
    return;
  }
  motorsTime = millis() + MOTORS_TICK;

  int delta = constrain(m1targetSpeed - m1speed, -MOTORS_ACCEL_PER_TICK, MOTORS_ACCEL_PER_TICK);
  m1speed += delta;
  delta = constrain(m2targetSpeed - m2speed, -MOTORS_ACCEL_PER_TICK, MOTORS_ACCEL_PER_TICK);
  m2speed += delta;

  digitalWrite(PIN_M1_A, m1speed > MOTORS_MIDDLE);
  digitalWrite(PIN_M1_B, m1speed < MOTORS_MIDDLE);
  analogWrite(PIN_M1_EN, motorsSpeedToPwm(m1speed));

  digitalWrite(PIN_M2_A, m2speed > MOTORS_MIDDLE);
  digitalWrite(PIN_M2_B, m2speed < MOTORS_MIDDLE);
  analogWrite(PIN_M2_EN, motorsSpeedToPwm(m2speed));
}

void motorsDisable() {
  pinMode(PIN_M1_EN, INPUT);
  pinMode(PIN_M2_EN, INPUT);
}

void motorsSetup() {
  analogWriteFreq(20000);

  pinMode(PIN_M1_A, OUTPUT);
  pinMode(PIN_M1_B, OUTPUT);
  pinMode(PIN_M1_EN, OUTPUT);

  pinMode(PIN_M2_A, OUTPUT);
  pinMode(PIN_M2_B, OUTPUT);
  pinMode(PIN_M2_EN, OUTPUT);

  m1speed = MOTORS_MIDDLE;
  m1targetSpeed = MOTORS_MIDDLE;
  m2speed = MOTORS_MIDDLE;
  m2targetSpeed = MOTORS_MIDDLE;
  motorsTime = 0;

  motorsTick();
}
