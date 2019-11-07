#include <BluetoothSerial.h>
#include <BtTerminalController.h>
#include <MemoryController.h>

void setup() {
  Serial.begin(115200);


  InitMemoryController();
  InitBtTerminalController();
  PrintNetworksFromMemoryCommand();
}

void loop() {
  ProcessBt();
  delay(20);
}
