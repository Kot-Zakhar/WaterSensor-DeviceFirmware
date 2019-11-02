#include <BluetoothSerial.h>
#include <Preferences.h>

#include <BtTerminalController.h>


BluetoothSerial SerialBT;
Preferences preferences;

// bt credentials
const char* bt_credentials_name = "ESP32";

void setup() {
  Serial.begin(115200);
  SerialBT.begin(bt_credentials_name);
  Serial.println("You can pair to ESP32s");

  preferences.begin(wifi_table_name, false);
  
  BtControllerSetup(&SerialBT, &preferences);

  PrintNetworksFromMemory(&preferences, NULL);

}

void loop() {
  ProcessBt();
  delay(20);
}
