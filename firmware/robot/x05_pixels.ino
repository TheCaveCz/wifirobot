#define PIXELS_TASK_INTERVAL 25UL
#define PIXELS_ANIM_MAX_RANGE 80

#define PIXELS_ANIM_BLUE 0
#define PIXELS_ANIM_VIOLET 1
#define PIXELS_ANIM_RED 2
#define PIXELS_ANIM_GREEN 3
#define PIXELS_ANIM_YELLOW 4
#define PIXELS_ANIM_NORMAL 5



uint8_t pixelsBuffer[3];

Ticker pixelsTicker;

uint16_t pixelsProgress;
uint8_t pixelsState;


void pixelsSet(uint8_t r, uint8_t g, uint8_t b) {
  pixelsBuffer[0] = r;
  pixelsBuffer[1] = g;
  pixelsBuffer[2] = b;

  ws2812Send(pixelsBuffer, 3);
}

void pixelsTick() {
  if (pixelsState == PIXELS_ANIM_NORMAL) {
    return;
  }

  uint8_t v = pixelsProgress < (PIXELS_ANIM_MAX_RANGE / 2)
              ? map(pixelsProgress, 0, (PIXELS_ANIM_MAX_RANGE / 2) - 1, 4, 128)
              : map(pixelsProgress, PIXELS_ANIM_MAX_RANGE / 2, PIXELS_ANIM_MAX_RANGE - 1, 128, 4);

  switch (pixelsState) {
    case PIXELS_ANIM_BLUE:
      pixelsSet(0, 0, v);
      break;
    case PIXELS_ANIM_VIOLET:
      pixelsSet(v, 0, v);
      break;
    case PIXELS_ANIM_GREEN:
      pixelsSet(0, v, 0);
      break;
    case PIXELS_ANIM_YELLOW:
      pixelsSet(v, v/2, 0);
      break;
    default:
      pixelsSet(v, 0, 0);
      break;
  }

  pixelsProgress++;
  if (pixelsProgress >= PIXELS_ANIM_MAX_RANGE) pixelsProgress = 0;
}

void pixelsSetAnimState(uint8_t v) {
  logValue("Pixels anim state set to ", v);
  pixelsState = v;
}

void pixelsSetup() {
  ws2812Setup();

  pixelsSet(0, 0, 0);

  pixelsProgress = 0;
  pixelsState = PIXELS_ANIM_BLUE;
  pixelsTicker.attach_ms(PIXELS_TASK_INTERVAL, pixelsTick);
}
