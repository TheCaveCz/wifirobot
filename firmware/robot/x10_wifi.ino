
#define MASTER_RESET_DELAY 20
#define MASTER_RESET_COUNTER 200

bool wifiShouldSaveConfig;

void wifiResetAndRestart() {
  logInfo("WiFi reset in progress");
  WiFi.disconnect(true);
  delay(1000);
  ESP.reset();
  while (1) delay(1);
}

void wifiStartAp() {
  WiFiManager wifiManager;
  wifiShouldSaveConfig = false;
  wifiManager.setSaveConfigCallback([]() {
    logInfo("Config should be saved");
    wifiShouldSaveConfig = true;
  });

  WiFiManagerParameter blynkTokenParam("token", "Blynk Auth token", config.blynkToken, 33);
  wifiManager.addParameter(&blynkTokenParam);

  String ssid = HOSTNAME_PREFIX;
  ssid += chipId;
  if (!wifiManager.startConfigPortal(ssid.c_str())) {
    logInfo("Failed to connect or setup, rebooting...");
    delay(1000);
    ESP.restart();
    while (1) delay(1);
  }

  strcpy(config.blynkToken, blynkTokenParam.getValue());

  if (wifiShouldSaveConfig) {
    configWrite();
  }
}

void wifiBail() {
  logInfo("Unable to connect");
  logValue("WiFi status: ", WiFi.status());
  pixelsSetAnimState(PIXELS_ANIM_RED);
  while (1) {
    adcTick(); // keep here to detect reset button hold
    delay(1);
  }
  // this function is not supposed to exit
}

void wifiSetup() {
  if (WiFi.getAutoConnect()) WiFi.setAutoConnect(false);
  if (WiFi.getMode() != WIFI_STA) WiFi.mode(WIFI_STA);

  if (WiFi.SSID() == "") {
    logInfo("No saved credentials");
    pixelsSetAnimState(PIXELS_ANIM_VIOLET);
    wifiStartAp();
    return;
  }

  logInfo("Connecting to WiFi...");
  logValue("Stored SSID: ", WiFi.SSID());
  pixelsSetAnimState(PIXELS_ANIM_BLUE);

  WiFi.begin();
  uint8_t connectCounter = 0;
  while (!WiFi.isConnected()) {
    connectCounter++;
    if (connectCounter > 150) {
      wifiBail();
    }
    delay(200);
  }

  logInfo("WiFi connected successfuly");
  logValue("Got IP: ", WiFi.localIP());
}
