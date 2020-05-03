#include <BtIO.h>

void BtInterruptCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);


BluetoothSerial BT;

void InitBtIO(const char *name) {
    BT.begin(name);
    BT.register_callback(BtInterruptCallback);
}

BluetoothSerial *GetCurrentBtSerial() {
  return &BT;
}

// IO

int AwaitAndReadBt(char *buffer, int maxLength){
  while (!BT.available()){}
  return ReadBt(buffer, maxLength);
}

int ReadBt(char *buffer, int maxLength){
  if (!BT.available())
      return -1;

  int realAmount = BT.available();

  log_d("%d symbols available", realAmount);

  int charAmount = BT.readBytes(buffer, realAmount < maxLength ? realAmount : maxLength);

  buffer[charAmount] = '\0';

  if (realAmount > charAmount)
    while (BT.available())
      BT.read();

  log_d("%s", buffer);

  return charAmount;
}

void WriteBt(const char* line){
  BT.write((const uint8_t *)line, strlen(line));
  Serial.println(line);
  delay(100);
}


// Interrupts

void BtInterruptCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if (event == ESP_SPP_SRV_OPEN_EVT){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Connected to bt.");
    IOIndicate(BT_CONNECTED);
  }
  if (event == ESP_SPP_CLOSE_EVT){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Disconnected from bt.");
    IOIndicate(BT_DISCONNECTED);
  }
}