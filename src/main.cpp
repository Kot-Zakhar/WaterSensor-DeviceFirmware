#include <BtController.h>
#include <MemoryController.h>
#include <InterruptController.h>
#include <IOController.h>
#include <SensorChecker.h>
#include <WifiController.h>

bool stateIsConfig;

void setup() {

  Serial.begin(115200);

  initIOController();
  initMemoryController();
  initWiFiController();

  stateIsConfig = isConfigStateInMemory();

  if (!stateIsConfig) {
    ioWrite(IO_WRITE_SCREEN, "Connecting to wifi...");
    stateIsConfig = (getWiFiCredentialsAmountFromMemory() == 0) || !connectToAnyWiFiFromMemory();
    if (stateIsConfig) {
      ioWrite(IO_WRITE_SCREEN, "Couldn't connect.");
    } else {
      ioWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, "Connected to ");
      ioWrite(IO_WRITE_SCREEN, getCurrentWiFiSsid().c_str());
      syncTime();
      char *buffer = (char *)malloc(STRING_LENGTH);
      ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, getDateTimeStr(buffer, STRING_LENGTH));
      free(buffer);
    }
  }

  if (stateIsConfig) {
    Serial.println("Config mode is on.");
    ioWrite(IO_WRITE_SCREEN, "Configuration mode.");
    ioIndicate(MODE_CONFIG_ON);
    wifiControllerOff();
    initBtController();
  } else {
    Serial.println("Working mode.");
    ioWrite(IO_WRITE_SCREEN, "Working mode.");
    ioIndicate(MODE_WORK_ON);
    initEmailController();
  }
  bindInterrupts(stateIsConfig);
  initSensorChecker(stateIsConfig);
}

void loop() {
  if (stateIsConfig){
    if (shouldBtBeProcessed()) {
      stopEmailChecker();
      unbindInterrupts();
      processBt();
      restartEmailChecker();
      rebindInterrupts();
    }
  } else {
    if (shouldEmailBeProcessed()) {
      stopEmailChecker();
      unbindInterrupts();
      processEmailController();
      restartEmailChecker();
      rebindInterrupts();
    }
  }
  processInterrupts();
  processSensorChecker();
  delay(100);
}
