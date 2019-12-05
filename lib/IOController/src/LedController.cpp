#include "IOController.h"
static const char *TAG = "LedController";

#define RED_LED_PIN 25
#define GREEN_LED_PIN 26
#define RGB_RED_LED_PIN 15
#define RGB_GREEN_LED_PIN 13
#define RGB_BLUE_LED_PIN 12

#define RED_LED_CHANNEL 0
#define GREEN_LED_CHANNEL 1
#define RGB_RED_LED_CHANNEL 2
#define RGB_GREEN_LED_CHANNEL 3
#define RGB_BLUE_LED_CHANNEL 4

#define LEDC_TIMER_13_BIT 13
#define LEDC_BASE_FREQ 5000
#define FADE_AMOUNT 5

void BindLEDs(){

    ledcSetup(RED_LED_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(RED_LED_PIN, RED_LED_CHANNEL);

    ledcSetup(GREEN_LED_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(GREEN_LED_PIN, GREEN_LED_CHANNEL);

    ledcSetup(RGB_RED_LED_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(RGB_RED_LED_PIN, RGB_RED_LED_CHANNEL);

    ledcSetup(RGB_GREEN_LED_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(RGB_GREEN_LED_PIN, RGB_GREEN_LED_CHANNEL);

    ledcSetup(RGB_BLUE_LED_CHANNEL, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    ledcAttachPin(RGB_BLUE_LED_PIN, RGB_BLUE_LED_CHANNEL);
}

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void BlinkLed(uint8_t ledChannel, int lightDuration){
    for (int i = 0; i < 255; i=i+2){
        ledcAnalogWrite(ledChannel, i);
        delay(2);
    }
    
    log_v("[%s] %s", TAG, "%d led is up", ledChannel);
    delay(lightDuration);
    for (int i = 254; i >= 0; i=i-2){
        ledcAnalogWrite(ledChannel, i);
        delay(2);
    }
    log_v("[%s] %s", TAG, "%d led is down", ledChannel);
}

void CheckLeds(int lightDuration){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SCREEN, "Checking leds:");
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "red led");
    BlinkLed(RED_LED_CHANNEL, lightDuration);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "green led");
    BlinkLed(GREEN_LED_CHANNEL, lightDuration);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "red rgb led");
    BlinkLed(RGB_RED_LED_CHANNEL, lightDuration);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "green rgb led");
    BlinkLed(RGB_GREEN_LED_CHANNEL, lightDuration);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "blue rgb led");
    BlinkLed(RGB_BLUE_LED_CHANNEL, lightDuration);
}