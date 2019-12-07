#include <BtTerminalController.h>
#include <MemoryController.h>
#include <InterruptController.h>
#include <IOController.h>

bool stateIsConfig;

// time syncing
// wifi connection 

void setup() {
  Serial.begin(115200);

  InitIOController();
  InitMemoryController();
  InitWiFiController();

  stateIsConfig = IsConfigStateInMemory() || GetWiFiCredentialsAmountFromMemory() == 0 || ConnectToAnyWiFiFromMemory();

  if (stateIsConfig){
    Serial.println("Config mode is on.");
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, "Configuration mode.");
    IOIndicate(MODE_CONFIG_ON);
    WiFiControllerOff();
    InitBtTerminalController();
  } else {
    Serial.println("Working mode.");
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, "Working mode.");
    IOIndicate(MODE_WORK_ON);
    // SendLetter(
    //   "First letter",
    //   "I've started to work. Everything is ok so far.",
    //   false
    // );
  }

  BindInterrupts(stateIsConfig);
}

void loop() {
  if (stateIsConfig){
    ProcessBt();
  } else {
  }
  ProcessInterrupts();
  delay(100);
}