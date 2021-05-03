#include <IOController.h>
#include <FastLED.h>
#include <Ticker.h>

#define LED_TIMER_INDEX 1
#define LED_TIMER_FREQ_DEVIDER 80
#define LED_UPDATE_FREQ 50
#define LED_PIN GPIO_NUM_13


const static uint16_t blinkDuration[2] = {300, 1000};

const static uint8_t ledPins[] = {25, 26};
const static uint8_t rgbLedPins[] = {15, 13, 12};

Ticker updater;

CRGB led;
// uint8_t brightness = 0;
// int currentColor = 0;

// HSVHue hueColors[] = {
//   HUE_RED,
//   HUE_GREEN,
//   HUE_BLUE,
//   HUE_ORANGE,
//   HUE_PURPLE
// };

portMUX_TYPE ledTimerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR ledUpdateInterrupt(){
    portENTER_CRITICAL_ISR(&ledTimerMux);
    portEXIT_CRITICAL_ISR(&ledTimerMux);
}

void initLED(){
    updater.attach_ms(LED_UPDATE_FREQ, ledUpdateInterrupt);
    FastLED.addLeds<WS2811, LED_PIN, GRB>(&led, 1);
    led = BLACK;
    // led = CHSV(HUE_RED, 255, 255);
    FastLED.show();
}
