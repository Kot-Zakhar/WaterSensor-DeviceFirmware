#include <IOController.h>
#include "LedController.cpp"

// void checkScreen();
// void initScreen();

// void initIOController(){
//   initLED();
  
//   initScreen();

//   checkScreen();

//   clearDisplay();
// }

void ioIndicate(message_t messageType){
    return;
    // switch(messageType){
    //     case BT_CONNECTED:
    //         blinkRGB(RGB_GREEN, FAST_BLINK);
    //         break;
    //     case BT_DISCONNECTED:
    //         blinkRGB(RGB_RED, FAST_BLINK);
    //         break;
    //     case BT_GOT_GOOD_COMMAND:
    //         blinkRGB(RGB_BLUE, FAST_BLINK);
    //         break;
    //     case BT_GOT_BAD_COMMAND:
    //         blinkRGB(RGB_MAGNETTA, FAST_BLINK);
    //         break;
    //     case BT_PENDING_COMMAND:
    //         blinkRGB(RGB_BLUE, BREATH, 1000);
    //         break;
    //     case BT_END_COMMAND:
    //         blinkRGB(RGB_WHITE, FAST_BLINK);
    //         break;

    //     case MODE_CONFIG_ON:
    //         greenLed.Reset();
    //         greenLed.Breathe(blinkDuration[BREATH]).Forever();
    //         break;
    //     case MODE_WORK_ON:
    //         greenLed.Reset();
    //         greenLed.On().Forever();
    //         break;

    //     case FATAL_ERROR:
    //         redLed.Reset();
    //         redLed.On();
    //         break;
    //     case ERROR:
    //         redLed.Reset();
    //         redLed.Breathe(blinkDuration[BREATH]);
    //         break;
    //     case WARNING:
    //         redLed.Reset();
    //         redLed.Blink(blinkDuration[FAST_BLINK], blinkDuration[FAST_BLINK]).Repeat(2);
    //         break;
    //     case SUCCESS:
    //         blinkRGB(RGB_GREEN, FAST_BLINK, 2);
    //         break;
    //     case WAIT:
    //         blinkRGB(RGB_PURPLE, BREATH, 100);
    //         break;

    //     case SensorValueCnaged:
    //         blinkRGB(RGB_GREEN, FAST_BLINK, 1);
    //         break;
    //     case SensorValueOutOfRange:
    //         blinkRGB(RGB_BLUE, FAST_BLINK, 3);
    //         break;

    //     case EMAIL_CHECKING:
    //         blinkRGB(RGB_YELLOW, BREATH, 1000);
    //         break;
    //     case EMAIL_MESSAGE_FOUND:
    //         blinkRGB(RGB_YELLOW, FAST_BLINK);
    //         break;
    //     case EMAIL_NO_MESSAGE_FOUND:
    //         blinkRGB(RGB_YELLOW, FAST_BLINK, 3);
    //         break;
    //     case EMAIL_SENDING_LETTER:
    //         blinkRGB(RGB_CYAN, BREATH, 100);
    //         break;

    //     case Interrupt0:
    //         log_v("Interrupt0");
    //         break;
    //     case Interrupt1:
    //         log_v("Interrupt1");
    //         break;
    //     case Interrupt2:
    //         log_v("Interrupt2");
    //         break;
    //     case Interrupt3:
    //         log_v("Interrupt3");
    //         break;
    //     default:
    //         break;
    // }
}
