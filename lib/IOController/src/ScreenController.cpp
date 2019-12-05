#include "IOController.h"
static const char *screenTag = "ScreenController";

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define MAX_LINES_AMOUNT 8

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void BindLEDs();
void CheckLeds(int delay);
void BlinkLed(uint8_t ledChannel, int interval);
void CheckScreen();

int currentScreenLinesAmount = 0;

void InitIOController(){
  BindLEDs();
  
  Wire.begin(5,4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)){
    log_e("[%s] %s", screenTag, "SSD1306 allocation failed");
    // todo: add sending error to ledController
    for(;;); // Don't proceed, loop forever
  }
  
  CheckScreen();
  CheckLeds(10);
  ClearDisplay();
}

void ClearDisplay(){
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  currentScreenLinesAmount = 0;
}

void IOWrite(int writeTo, const char *str){
  if (writeTo & IO_WRITE_SERIAL){
    log_i("[%s] %s", screenTag, str);
  }
  if (writeTo & IO_WRITE_SCREEN){
    if ((writeTo & IO_WRITE_CLEAN_BEFORE_WRITE) || (currentScreenLinesAmount == MAX_LINES_AMOUNT))
      ClearDisplay();
    display.println(str);
    display.display();
    delay(50);
    currentScreenLinesAmount++;
  }
}

void CheckScreen(){
  ClearDisplay();

  display.println("Screen check...");
  display.display();

  delay(500);
}