#define CONTROL_PACKET_MAGIC 0x63
#define CONTROL_CMD_MOVE 1
#define CONTROL_CMD_DISCOVER 2
#define CONTROL_CMD_DISCOVER_RESPONSE 3
#define CONTROL_CMD_LIGHT 4

WiFiUDP controlUdp;
uint8_t controlBuffer[32];

bool controlCheckChipId() {
  uint32_t id = controlBuffer[5];
  id <<= 8;
  id |= controlBuffer[4];
  id <<= 8;
  id |= controlBuffer[3];
  id <<= 8;
  id |= controlBuffer[2];
  return id == ESP.getChipId();
}

void controlSendDiscoverResponse() {
  uint8_t buf[6];
  memset(buf, 0, sizeof(buf));
  buf[0] = CONTROL_PACKET_MAGIC;
  buf[1] = CONTROL_CMD_DISCOVER_RESPONSE;
  buf[2] = ESP.getChipId() & 0xff;
  buf[3] = (ESP.getChipId() >> 8) & 0xff;
  buf[4] = (ESP.getChipId() >> 16) & 0xff;
  buf[5] = (ESP.getChipId() >> 24) & 0xff;

  controlUdp.beginPacket(controlUdp.remoteIP(), controlUdp.remotePort());
  controlUdp.write(buf, sizeof(buf));
  controlUdp.endPacket();
}

void controlProcessBuffer() {
  if (controlBuffer[0] != CONTROL_PACKET_MAGIC) {
    logValue("Control packet magic mismatch: ", controlBuffer[0]);
    return;
  }

  switch (controlBuffer[1]) {
    case CONTROL_CMD_MOVE:
      if (controlCheckChipId()) {
        logicSetSpeed(controlBuffer[6], controlBuffer[7]);
      }
      break;

    case CONTROL_CMD_DISCOVER:
      controlSendDiscoverResponse();
      break;

    case CONTROL_CMD_LIGHT:
      if (controlCheckChipId()) {
        logicSetLight(controlBuffer[7], controlBuffer[8], controlBuffer[9]);
        logicSetLightOn(controlBuffer[6]);
      }
      break;

    default: {
        logValue("Control unknown command: ", controlBuffer[1]);
        break;
      }
  }
}

void controlTick() {
  int len = controlUdp.parsePacket();
  if (len == 0) return;

  memset(controlBuffer, 0, sizeof(controlBuffer));
  len = controlUdp.read(controlBuffer, sizeof(controlBuffer) - 1);
  if (len <= 0) return;
  controlProcessBuffer();
}

void controlSetup() {
  controlUdp.begin(UDP_PORT);
}
