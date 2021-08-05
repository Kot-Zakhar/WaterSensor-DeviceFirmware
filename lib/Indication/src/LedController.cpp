#include <IOController.h>
#include <Ticker.h>
#include "LedController.h"

// #define LED_TIMER_INDEX 1
// #define LED_TIMER_FREQ_DEVIDER 80
// #define LED_UPDATE_FREQ 50
#define GREEN_LED_PIN GPIO_NUM_13
#define RED_LED_PIN GPIO_NUM_12

// CRGB prevColor;
// CRGB led;
// uint8_t brightness = 0;
// int currentColor = 0;

// HSVHue hueColors[] = {
//   HUE_RED,
//   HUE_GREEN,
//   HUE_BLUE,
//   HUE_ORANGE,
//   HUE_PURPLE
// };

void initLED() {
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    redLedOn();
    greenLedOn();
    delay(500);
    redLedOff();
    greenLedOff();
    // FastLED.addLeds<WS2811, LED_PIN, GRB>(&led, 1);
    // led = RGB_GREEN;
    // FastLED.show();
}


void redLedOn() {
    digitalWrite(RED_LED_PIN, HIGH);
}

void redLedOff() {
    digitalWrite(RED_LED_PIN, LOW);
}

void greenLedOn() {
    digitalWrite(GREEN_LED_PIN, HIGH);
}

void greenLedOff() {
    digitalWrite(GREEN_LED_PIN, LOW);
}
