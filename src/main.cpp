#include <BtController.h>
#include <MemoryController.h>
#include <InterruptController.h>
#include <IndicationController.h>
#include <SensorsChecker.h>
#include <WifiController.h>
#include <GsmService.h>
#include <EmailChecker.h>
#include <WifiHotspotController.h>

device_state_t currentState;

void setup() {

  Serial.begin(115200);

  initIndicationController();

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
    Serial.printf("Config mode. %s\n", currentState & DEVICE_STATE_CONFIG_BLUETOOTH ? "Bluetooth" : "WiFi");
    if (currentState & DEVICE_STATE_CONFIG_BLUETOOTH)
      initBtController();
    else if (currentState & DEVICE_STATE_CONFIG_WIFI_HOTSPOT)
      initWiFiHotspot();
    else {
      Serial.println("Unknown state");
      setStateInMemory(DEFAULT_DEVICE_CONFIG_STATE);
      esp_restart();
    }

  } else {
    Serial.println("Working mode.");
    startEmailChecker();
  }
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
  case DEVICE_STATE_WORK:
    if (emailNeedToBeProcessed()) {
      detachInterruptsAndTimers();
      processEmailChecker();
      reattachInterruptsAndTimers();
    }
    break;
    
  case DEVICE_STATE_CONFIG_BLUETOOTH:
    if (btNeedToBeProcessed()) {
      detachInterruptsAndTimers();
      processBt();
      reattachInterruptsAndTimers();
    }
    break;
  case DEVICE_STATE_CONFIG_WIFI_HOTSPOT:
    /* there is going to be wifi hotspot processing */
    break;
  
  default:
    break;
  }

  if (sensorsNeedToBeProcessed()) {
    detachInterruptsAndTimers();
    processSensorsChecker();
    reattachInterruptsAndTimers();
  }

  if (gsmNeedToBeProcessed()) {
    detachInterruptsAndTimers();
    processGsm();
    reattachInterruptsAndTimers();
  }
  
  delay(100);
}
