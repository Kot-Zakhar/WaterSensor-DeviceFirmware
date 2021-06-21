#include <BtController.h>

const char* bt_credentials_name = "ESP32 Water Sensor";

void initBtController(){
  initBtIO(bt_credentials_name);
  // ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, ("BT - " + String(bt_credentials_name)).c_str());
  log_d("Initializing bt: %s", bt_credentials_name);
}

bool btNeedToBeProcessed() {
  return btAvailable() > 0;
}

void processBt() {
  int length = btAvailable();
  if (length <= 0)
    return;

  char *message = (char *)malloc(length + 1);
  readBt(message, length + 1);

  char *response = (char *)malloc(STRING_LENGTH);

  response = processMessage(message, length, response, STRING_LENGTH);

  writeBt(response);

  free(response);
  free(message);
  log_d("Bt command processing is finished.");
}