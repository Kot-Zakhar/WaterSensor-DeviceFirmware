#include <BtIO.h>

void btInterruptCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);


BluetoothSerial BT;

void initBtIO(const char *name) {
  BT.begin(name);
  BT.register_callback(btInterruptCallback);
}

BluetoothSerial *getCurrentBtSerial() {
  return &BT;
}

int btAvailable() {
  return BT.available();
}


// IO

int awaitAndReadBt(char *buffer, int maxLength){
  while (!BT.available()){}
  return readBt(buffer, maxLength);
}

int readBt(char *buffer, int maxLength){
  if (!BT.available())
      return -1;

  int realAmount = BT.available();

  int ableToRead = realAmount < maxLength - 1 ? realAmount : maxLength - 1;

  int charAmount = BT.readBytes(buffer, ableToRead);

  buffer[charAmount] = '\0';

  if (realAmount > ableToRead){
    // TODO: reallocate space if realAmount > charAmount
    char *tail = (char *) malloc(realAmount - ableToRead + 1);
    int tailLength = BT.readBytes(tail, realAmount - ableToRead);
    tail[tailLength] = '\0';
    log_e("Cutting the tail of the message: '%s'", tail);
    free(tail);
  }

  log_d("%s", buffer);

  return charAmount;
}

void writeBt(const char* line){
  BT.write((const uint8_t *)line, strlen(line));
  Serial.println(line);
  delay(100);
}


// Interrupts

void btInterruptCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if (event == ESP_SPP_SRV_OPEN_EVT){
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Connected to bt.");
    ioIndicate(BT_CONNECTED);
  }
  if (event == ESP_SPP_CLOSE_EVT){
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Disconnected from bt.");
    ioIndicate(BT_DISCONNECTED);
  }
}