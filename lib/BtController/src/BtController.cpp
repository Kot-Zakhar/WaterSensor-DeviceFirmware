#include <BtController.h>

const char* bt_credentials_name = "ESP32";

void InitBtController(){
  InitBtIO(bt_credentials_name);
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("BT - " + String(bt_credentials_name)).c_str());
}

void ProcessBt() {
  int length = BtAvailable();
  if (length <= 0)
    return;

  char *message = (char *)malloc(length + 1);
  ReadBt(message, length + 1);

  // DynamicJsonDocument *jsonDoc = new DynamicJsonDocument(length);
  DynamicJsonDocument jsonDoc(length);
  DeserializationError err = deserializeJson(jsonDoc, message);
  if (err) {
    log_d("It's a bt terminal message.");
    ProcessBtTerminalMessage(message, length);
  } else {
    log_d("It's a bt json message.");
    JsonObject obj = jsonDoc.as<JsonObject>();
    ProcessBtJsonMessage(obj);
  }

  // delete(jsonDoc);
  free(message);
  log_d("Bt command processing is finished.");
}