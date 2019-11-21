#include <BtTerminalController.h>

void setup() {
  Serial.begin(115200);


  InitMemoryController();
  InitBtTerminalController();
  WiFiControllerInit();
  PrintNetworksFromMemoryCommand();

  ConnectToWiFi("KOTPC", "6V2490a3");
  AwaitForWiFiConnection();

  Serial.print("CONFIG_SW_COEXIST_ENABLE = ");
  Serial.println(String(CONFIG_SW_COEXIST_ENABLE));
  SendLetter(
    String("kot.zakhar@gmail.com"),
    String("New email"),
    String("Hello, this is a new message from esp32."),
    false
  );
}

void loop() {
  ProcessBt();
  delay(100);
}
