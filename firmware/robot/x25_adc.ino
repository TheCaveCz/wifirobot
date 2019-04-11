#define ADC_TICK 500
#define ADC_BUTTON_TRESHOLD 20
#define ADC_BUTTON_TICKS 6

uint32_t adcTime;
uint16_t adcBattery;
uint8_t adcButtonCounter;

void adcSetup() {
  adcTime = 0;
  adcBattery = 0;
  adcButtonCounter = 0;
}


void adcTick() {
  if (millis() < adcTime) {
    return;
  }
  adcTime = millis() + ADC_TICK;

  int v = analogRead(A0);
  if (v < ADC_BUTTON_TRESHOLD) {
    adcButtonCounter++;
    if (adcButtonCounter > ADC_BUTTON_TICKS) {
      wifiResetAndRestart();
    }
    return;
  }
  adcButtonCounter = 0;

  int bat = map(v, 0, 1023, 0, 510);
  if (bat != adcBattery) {
    adcBattery = bat;
    logValue("Battery: ", adcBattery);
    if (Blynk.connected()) {
      Blynk.virtualWrite(BLYNK_BATTERY_PERCENT_PIN, map(constrain(adcBattery, 300, 450), 300, 450, 0, 100));
      Blynk.virtualWrite(BLYNK_BATTERY_VOLTAGE_PIN, adcBattery / 100.0);
    }
  }
}
