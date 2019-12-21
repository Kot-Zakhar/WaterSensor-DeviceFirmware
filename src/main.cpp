#include <BtTerminalController.h>
#include <MemoryController.h>
#include <InterruptController.h>
#include <IOController.h>
#include <SensorChecker.h>
#include <WifiController.h>

bool stateIsConfig;

void setup() {

  Serial.begin(115200);

  InitIOController();
  InitMemoryController();
  InitWiFiController();

  stateIsConfig = IsConfigStateInMemory();

  if (!stateIsConfig) {
    IOWrite(IO_WRITE_SCREEN, "Connecting to wifi...");
    stateIsConfig = (GetWiFiCredentialsAmountFromMemory() == 0) || !ConnectToAnyWiFiFromMemory();
    if (stateIsConfig) {
      IOWrite(IO_WRITE_SCREEN, "Couldn't connect.");
    } else {
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, "Connected to ");
      IOWrite(IO_WRITE_SCREEN, GetCurrentWiFiSsid().c_str());
      SyncTime();
      char *buffer = (char *)malloc(STRING_LENGTH);
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, GetDateTimeStr(buffer, STRING_LENGTH));
      free(buffer);
    }
  }

  if (stateIsConfig) {
    Serial.println("Config mode is on.");
    IOWrite(IO_WRITE_SCREEN, "Configuration mode.");
    IOIndicate(MODE_CONFIG_ON);
    WiFiControllerOff();
    InitBtTerminalController();
  } else {
    Serial.println("Working mode.");
    IOWrite(IO_WRITE_SCREEN, "Working mode.");
    IOIndicate(MODE_WORK_ON);
    // SendLetter(
    //   "First letter",
    //   "I've started to work. Everything is ok so far.",
    //   false
    // );
    InitEmailController();
  }
  BindInterrupts(stateIsConfig);
  InitSensorChecker(stateIsConfig);
}

void loop() {
  if (stateIsConfig){
    ProcessBt();
  } else {
    ProcessEmailController();
  }
  ProcessInterrupts();
  ProcessSensorChecker();
  delay(100);
}
