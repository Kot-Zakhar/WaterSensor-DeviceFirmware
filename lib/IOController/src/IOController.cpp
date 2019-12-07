#include "IOController.h"

void BindLEDs();
void BlinkRGB(rgb_color_t color, blink_duration_t blinkDurationType, int times = 1);
void CheckLeds();
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
