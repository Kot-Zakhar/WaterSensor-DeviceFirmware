#include "ScreenController.h"
#include "WifiHotspotController.h"
#include "WifiController.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int currentScreenLinesAmount = 0;
screen_page_t currentPage = SCREEN_PAGE_PREVIEW;

void setPage(screen_page_t page) {
  currentPage = page;
}

screen_page_t getPage() {
  return currentPage;
}

void clearDisplay(){
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  currentScreenLinesAmount = 0;
}

// void ioWrite(int writeTo, const char *str){
//   if (writeTo & IO_WRITE_SERIAL){
//     log_i("%s", str);
//   }
//   delay(50);
//   if (writeTo & IO_WRITE_SCREEN){
//     if ((writeTo & IO_WRITE_CLEAN_BEFORE_WRITE) || (currentScreenLinesAmount == MAX_LINES_AMOUNT))
//       clearDisplay();
//     display.println(str);
//     display.display();
//     delay(50);
//     currentScreenLinesAmount++;
//   }
// }

// void checkScreen(){
//   clearDisplay();

//   display.println("Screen check...");
//   display.display();

//   delay(500);
// }

void initScreen(){

  Wire.begin(5,4);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)){
    log_e("SSD1306 allocation failed");
    // ioIndicate(FATAL_ERROR);
    // todo: add sending error to ledController
    for(;;); // Don't proceed, loop forever
  }

  clearDisplay();
  display.println("Initializing...");
  display.display();
}

void showPreviewPage(device_state_t currentState) {
  display.println("Preview page");
  switch (currentState)
  {
  case DEVICE_STATE_CONFIG_BLUETOOTH:
    display.println("Bluetooth mode");
    display.println("ESP32 Water Sensor");
    break;
  case DEVICE_STATE_CONFIG_WIFI_HOTSPOT:
    display.println("WiFi HotSpot");
    display.println("ESP32 Water Sensor");
    display.println("No password");
    display.printf("%s\r\n", getHotspotIP().toString().c_str());    
    break;
  case DEVICE_STATE_WORK_WIFI_ALWAYS_CONNECTED:
    display.println("WiFi: always");
    display.println(getCurrentWiFiSsid().c_str());
    display.println(getCurrentIP().toString().c_str());
    break;
  case DEVICE_STATE_WORK_WIFI_FOR_NOTIFICATION:
    display.println("WiFi: notif only");
    break;
  default:
    break;
  }
}

void showWiFiRecords() {}
void showNetworkSettings() {}
void showImapSettings() {}
void showSmtpSettings() {}
void showEmailRecipients() {}
void showGsmSettings() {}
void showGprsSettings() {}
void showGsmRecipients() {}

void updateScreen(device_state_t currentState) {
  clearDisplay();
  
  switch (currentPage)
  {
  case SCREEN_PAGE_PREVIEW:
    showPreviewPage(currentState);
    break;
  case SCREEN_PAGE_WIFI_RECORDS:
    showWiFiRecords();
    break;
  case SCREEN_PAGE_NETWORK_SETTINGS:
    showNetworkSettings();
    break;
  case SCREEN_PAGE_IMAP_SETTINGS:
    showImapSettings();
    break;
  case SCREEN_PAGE_SMTP_SETTINGS:
    showSmtpSettings();
    break;
  case SCREEN_PAGE_EMAIL_RECIPIENTS:
    showEmailRecipients();
    break;
  case SCREEN_PAGE_GSM_SETTINGS:
    showGsmSettings();
    break;
  case SCREEN_PAGE_GPRS_SETTINGS:
    showGprsSettings();
    break;
  case SCREEN_PAGE_GSM_RECIPIENTS:
    showGsmRecipients();
    break;
  default:
    showPreviewPage(currentState);
    break;
  }
  
  display.display();
}