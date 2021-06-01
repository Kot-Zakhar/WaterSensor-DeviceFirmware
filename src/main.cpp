#include <BtController.h>
#include <MemoryController.h>
#include <InterruptController.h>
#include <SensorsChecker.h>
#include <WifiController.h>
#include <GsmService.h>
#include <EmailChecker.h>
#include <WifiHotspotController.h>
#include <HttpServerController.h>
#include <ScreenController.h>

device_state_t currentState;

void setup() {

  Serial.begin(115200);
  Serial.println("Starting");

  initScreen();

  initMemoryController();
  initGsmService();

  currentState = getStateFromMemory();

  // checking availability
  if (currentState & DEVICE_STATE_WORK) {
    bool ableToWork =
      (GsmNotificationIsOn() && GsmConnectionIsAvailable()) ||
      (GprsNotificationIsOn() && GpssConnectionIsAvailable()) ||
      (WiFiNotificationIsOn() && WiFiConnectionIsAvailable());
    if (!ableToWork)
      currentState = getPreferredConfigStateFromMemory();
  }

  initSensorsChecker();
  initButtonsInterrupts(currentState & DEVICE_STATE_CONFIG);

  if (currentState & DEVICE_STATE_CONFIG) {
    Serial.printf("Config mode. %d %s\n", currentState, currentState == DEVICE_STATE_CONFIG_BLUETOOTH ? "Bluetooth" : "WiFi");
    if (currentState == DEVICE_STATE_CONFIG_BLUETOOTH)
      initBtController();
    else if (currentState == DEVICE_STATE_CONFIG_WIFI_HOTSPOT) {
      initWiFiHotspot();
      initHttpServer();
    } else  {
      Serial.println("Unknown state");
      setStateInMemory(DEFAULT_DEVICE_CONFIG_STATE);
      esp_restart();
    }

  } else {
    Serial.println("Working mode.");
    initWiFiController();
    if (currentState == DEVICE_STATE_WORK_WIFI_ALWAYS_CONNECTED) {
      if (!connectToAnyWiFiFromMemory()) {
        log_d("can't connect to any wifi network");
        setStateInMemory(DEFAULT_DEVICE_CONFIG_STATE);
        delay(1000);
        esp_restart();
      }
      startEmailChecker();
      initHttpServer();
      // connectToWiFi("KotFi", "randomPass125739");
      // connectToWiFi("Kot-Fi", "18.12.1999");
      // awaitForWiFiConnection();
    } else if (currentState == DEVICE_STATE_WORK_WIFI_FOR_NOTIFICATION) {

    }
  }

  Serial.println("Finishing setup");
}

void detachInterruptsAndTimers() {
  pauseEmailChecker();
  unbindInterrupts();
}

void reattachInterruptsAndTimers() {
  resumeEmailChecker();
  rebindInterrupts();
}

void loop() {
  processButtons();

  switch (currentState)
  {    
  case DEVICE_STATE_CONFIG_BLUETOOTH:
    if (btNeedToBeProcessed()) {
      detachInterruptsAndTimers();
      processBt();
      reattachInterruptsAndTimers();
    }
    break;
  case DEVICE_STATE_CONFIG_WIFI_HOTSPOT:
    /* there is going to be wifi hotspot processing */
    processWiFiHotspot();
    break;
  case DEVICE_STATE_WORK_WIFI_ALWAYS_CONNECTED:
    if (emailNeedToBeProcessed()) {
      detachInterruptsAndTimers();
      processEmailChecker();
      reattachInterruptsAndTimers();
    }
    break;
  case DEVICE_STATE_WORK_WIFI_FOR_NOTIFICATION:
  default:
    break;
  }

  // if (sensorsNeedToBeProcessed()) {
  //   detachInterruptsAndTimers();
  //   processSensorsChecker();
  //   reattachInterruptsAndTimers();
  // }

  if (gsmNeedToBeProcessed()) {
    detachInterruptsAndTimers();
    processGsm();
    reattachInterruptsAndTimers();
  }

  updateScreen(currentState);

  delay(100);
}
