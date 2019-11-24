#include <BtTerminalController.h>
#include <MemoryController.h>
#include <InterruptController.h>

bool stateIsConfig;

void setup() {
  Serial.begin(115200);


  InitMemoryController();
  WiFiControllerInit();

  stateIsConfig = IsConfigStateInMemory() || GetWiFiCredentialsAmountFromMemory() == 0 || !ConnectToAnyWiFiFromMemory();

  if (stateIsConfig){
    Serial.println("Config mode is on.");
    WiFiControllerOff();
    InitBtTerminalController();
  } else {
    Serial.println("Working mode.");
    SendLetter(
      "First letter",
      "I've started to work. Everything is ok so far.",
      false
    );
  }

  BindInterrupts(stateIsConfig);
}

void loop() {
  if (stateIsConfig){
    ProcessBt();
  } else {
  }
  ProcessInterrupts(stateIsConfig);
  delay(100);
}