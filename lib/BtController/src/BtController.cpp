#include <BtController.h>

const char* bt_credentials_name = "ESP32";

void initBtController(){
  initBtIO(bt_credentials_name);
  ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("BT - " + String(bt_credentials_name)).c_str());
}

bool shouldBtBeProcessed() {
  return btAvailable() > 0;
}

void processBt() {
  int length = btAvailable();
  if (length <= 0)
    return;

  char *message = (char *)malloc(length + 1);
  readBt(message, length + 1);

  // DynamicJsonDocument *jsonDoc = new DynamicJsonDocument(length);
  DynamicJsonDocument jsonDoc(length);
  DeserializationError err = deserializeJson(jsonDoc, message);
  if (err) {
    log_d("It's a bt terminal message.");
    processBtTerminalMessage(message, length);
  } else {
    log_d("It's a bt json message.");
    JsonObject obj = jsonDoc.as<JsonObject>();
    processBtJsonMessage(obj);
  }

  // delete(jsonDoc);
  free(message);
  log_d("Bt command processing is finished.");
}