
void setup() {
  Serial.begin(115200);

  logInfo("Starting");
  logValue("Chip id: ", chipId);

  pixelsSetup();
  configSetup();
  motorsSetup();
  adcSetup();
  otaSetup();
  logicSetup();
  controlSetup();
  wifiSetup();
  blynkSetup();
  logInfo("Ready to go!");
}

void loop() {
  ArduinoOTA.handle();
  blynkTick();
  adcTick();
  motorsTick();
  controlTick();
}
