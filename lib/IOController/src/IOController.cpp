#include "IOController.h"
#include "LedController.cpp"

void CheckScreen();
void InitDisplay();

void InitIOController(){
  BindLEDs();
  
  InitDisplay();

  CheckScreen();
  CheckLeds();

  ClearDisplay();
}

void IOIndicate(message_t messageType){
    switch(messageType){
        case BT_CONNECTED:
            BlinkRGB(RGB_GREEN, FAST_BLINK);
            break;
        case BT_DISCONNECTED:
            BlinkRGB(RGB_RED, FAST_BLINK);
            break;
        case BT_GOT_GOOD_COMMAND:
            BlinkRGB(RGB_BLUE, FAST_BLINK);
            break;
        case BT_GOT_BAD_COMMAND:
            BlinkRGB(RGB_MAGNETTA, FAST_BLINK);
            break;
        case BT_PENDING_COMMAND:
            BlinkRGB(RGB_BLUE, BREATH, 1000);
            break;
        case BT_END_COMMAND:
            BlinkRGB(RGB_WHITE, FAST_BLINK);
            break;

        case MODE_CONFIG_ON:
            greenLed.Reset();
            greenLed.Breathe(blinkDuration[BREATH]).Forever();
            break;
        case MODE_WORK_ON:
            greenLed.Reset();
            greenLed.On().Forever();
            break;

        case FATAL_ERROR:
            redLed.Reset();
            redLed.On();
            break;
        case ERROR:
            redLed.Reset();
            redLed.Breathe(blinkDuration[BREATH]);
            break;
        case WARNING:
            redLed.Reset();
            redLed.Blink(blinkDuration[FAST_BLINK], blinkDuration[FAST_BLINK]).Repeat(2);
            break;
        case SUCCESS:
            BlinkRGB(RGB_GREEN, FAST_BLINK, 2);
            break;
        case WAIT:
            BlinkRGB(RGB_PURPLE, BREATH, 100);
            break;

        case Interrupt0:
            log_v("Interrupt0");
            break;
        case Interrupt1:
            log_v("Interrupt1");
            break;
        case Interrupt2:
            log_v("Interrupt2");
            break;
        case Interrupt3:
            log_v("Interrupt3");
            break;
        default:
            break;
    }
}
