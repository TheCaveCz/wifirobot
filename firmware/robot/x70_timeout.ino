
uint32_t timeoutTime;


void timeoutRefresh() {
  timeoutTime = millis() + TIMEOUT_INTERVAL;
}

void timeoutClear() {
  timeoutTime = 0;
}

bool timeoutCheck() {
  if (timeoutTime && (millis() > timeoutTime)) {
    logInfo("Timeout reached, disabling animations");
    timeoutClear();
    return true;
  } else {
    return false;
  }
}

bool timeoutIsActive() {
  return timeoutTime != 0;
}
