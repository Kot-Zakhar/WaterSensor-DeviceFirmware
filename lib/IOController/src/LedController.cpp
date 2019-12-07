#include "IOController.h"
#include <jled.h>

#define LED_TIMER_INDEX 1
#define LED_TIMER_FREQ_DEVIDER 80

enum led_t {
    RED_LED,
    GREEN_LED,
    RGB_LED,
};

enum rgb_led_t {
    RGB_RED_LED,
    RGB_GREEN_LED,
    RGB_BLUE_LED,
};

struct rgb_color_code_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


const static rgb_color_code_t rgbColorCodes[13] = {
    { 2, 2, 2 },
    { 2, 2, 0 },
    { 2, 1, 0 },
    { 2, 0, 0 },
    { 2, 0, 1 },
    { 2, 0, 2 },
    { 1, 0, 2 },
    { 0, 0, 2 },
    { 0, 1, 2 },
    { 0, 2, 2 },
    { 0, 2, 1 },
    { 0, 2, 0 },
    { 1, 2, 0 }
};

const static uint16_t blinkDuration[2] = {300, 1000};

const static uint8_t ledPins[] = {25, 26};
const static uint8_t rgbLedPins[] = {15, 13, 12};

hw_timer_t *timer = NULL;

JLed redLed(ledPins[RED_LED]);
JLed greenLed(ledPins[GREEN_LED]);

JLed rgbLeds[] = {
    JLed(rgbLedPins[RGB_RED_LED]),
    JLed(rgbLedPins[RGB_GREEN_LED]),
    JLed(rgbLedPins[RGB_BLUE_LED])
};

JLedSequence rgbLed(JLedSequence::eMode::PARALLEL, rgbLeds);

portMUX_TYPE ledTimerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR LedUpdateInterrupt(){
    portENTER_CRITICAL_ISR(&ledTimerMux);
    rgbLed.Update();
    redLed.Update();
    greenLed.Update();
    portEXIT_CRITICAL_ISR(&ledTimerMux);
}

void BindLEDs(){
    timer = timerBegin(LED_TIMER_INDEX, LED_TIMER_FREQ_DEVIDER, true);
    timerAttachInterrupt(timer, &LedUpdateInterrupt, true);
    timerAlarmWrite(timer, 10000, true);
    timerAlarmEnable(timer);
    delay(100);
}

void BlinkRGB(rgb_color_t color, blink_duration_t blinkDurationType, int times = 1){
    portENTER_CRITICAL(&ledTimerMux);
    rgbLed.Stop();
    rgbLeds[RGB_RED_LED]
        .Breathe((rgbColorCodes[color].r > 0 ? 1 : 0) * blinkDuration[blinkDurationType])
        .Repeat(times)
        .DelayBefore(0)
        .DelayAfter((rgbColorCodes[color].r > 0 ? 2 : 1) * blinkDuration[blinkDurationType]);
    rgbLeds[RGB_GREEN_LED]
        .Breathe((rgbColorCodes[color].g > 0 ? 1 : 0) * blinkDuration[blinkDurationType])
        .Repeat(times)
        .DelayBefore(0)
        .DelayAfter((rgbColorCodes[color].g > 0 ? 2 : 1) * blinkDuration[blinkDurationType]);
    rgbLeds[RGB_BLUE_LED]
        .Breathe((rgbColorCodes[color].b > 0 ? 1 : 0) * blinkDuration[blinkDurationType])
        .Repeat(times)
        .DelayBefore(0)
        .DelayAfter((rgbColorCodes[color].b > 0 ? 2 : 1) * blinkDuration[blinkDurationType]);
    rgbLed.Reset();
    portEXIT_CRITICAL(&ledTimerMux);
}

void BlinkLed(led_t led, blink_duration_t blinkDurationType, int times = 1){
    portENTER_CRITICAL(&ledTimerMux);
    rgbLeds[led]
        .Breathe(blinkDuration[blinkDurationType])
        .Repeat(times)
        .DelayBefore(0)
        .DelayAfter(blinkDuration[blinkDurationType]);
    portEXIT_CRITICAL(&ledTimerMux);
}

void CheckLeds(){
    portENTER_CRITICAL(&ledTimerMux);
    greenLed.Breathe(500).DelayAfter(2000);
    rgbLed.Stop();
    rgbLeds[RGB_RED_LED].DelayBefore(500).Breathe(500).DelayAfter(1500);
    rgbLeds[RGB_GREEN_LED].DelayBefore(1000).Breathe(500).DelayAfter(1000);
    rgbLeds[RGB_BLUE_LED].DelayBefore(1500).Breathe(500).DelayAfter(500);
    rgbLed.Reset();
    redLed.DelayBefore(2000).Breathe(500);
    portEXIT_CRITICAL(&ledTimerMux);
}