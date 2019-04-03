
void otaSetup() {
  String name = "The Cave Robot (";
  name += chipId;
  name += ")";

  MDNS.setInstanceName(name);

  String ssid = HOSTNAME_PREFIX;
  ssid += chipId;
  ArduinoOTA.setHostname(ssid.c_str());

#ifdef OTA_PASSWORD
  ArduinoOTA.setPassword(OTA_PASSWORD);
#endif

  ArduinoOTA.onStart([]() {
    pixelsSetAnimState(PIXELS_ANIM_YELLOW);
    logicStop();
    motorsDisable();
  });
  ArduinoOTA.onEnd([]() {
    pixelsSetAnimState(PIXELS_ANIM_NORMAL);
    pixelsSet(80, 40, 0);
  });
  ArduinoOTA.begin();
}
