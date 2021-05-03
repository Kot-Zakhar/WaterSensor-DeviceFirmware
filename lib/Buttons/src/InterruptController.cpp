#include <InterruptController.h>

static const char *memory_empty_message = "No networks in memory";


bool isConfigState;

volatile bool buttonsPressed[] = {
  false,
  false,
  false,
  false
};

volatile uint8_t buttonsPins[] = {
  39,
  16,
  2,
  14,
};

portMUX_TYPE interruptMux = portMUX_INITIALIZER_UNLOCKED;


void ListSMTPSettings();
void SendLetterAboutButtonPressed();
void ListWiFiFromMemory();


void IRAM_ATTR Button0Interrupt(){
  if (!interruptMux.count){
    portENTER_CRITICAL_ISR(&interruptMux);
    buttonsPressed[0] = true;
    portEXIT_CRITICAL_ISR(&interruptMux);
  }
  delay(100);
}

void IRAM_ATTR Button1Interrupt(){
  if (!interruptMux.count){
    portENTER_CRITICAL_ISR(&interruptMux);
    buttonsPressed[1] = true;
    portEXIT_CRITICAL_ISR(&interruptMux);
  }
  delay(100);
}

void IRAM_ATTR Button2Interrupt(){
  if (!interruptMux.count){
    portENTER_CRITICAL_ISR(&interruptMux);
    buttonsPressed[2] = true;
    portEXIT_CRITICAL_ISR(&interruptMux);
  }
  delay(100);
}

void IRAM_ATTR Button3Interrupt(){
  if (!interruptMux.count){
    portENTER_CRITICAL_ISR(&interruptMux);
    buttonsPressed[3] = true;
    portEXIT_CRITICAL_ISR(&interruptMux);
  }
  delay(100);
}

void ProcessInterrupt(int index){
  bool currentSavedStateIsConfig;
  switch(index){
    case 0:
      ioIndicate(Interrupt0);
      if (isConfigState){
        ListSMTPSettings();
      } else {
        SendLetterAboutButtonPressed();
      }
      break;
    case 1:
      ioIndicate(Interrupt1);
      ListWiFiFromMemory();
      break;
    case 2:
      ioIndicate(Interrupt2);
      delay(100);
      currentSavedStateIsConfig = isConfigStateInMemory();
      setStateInMemory(!currentSavedStateIsConfig);
      ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, (String("After restart: ") + (currentSavedStateIsConfig ? "work" : "config")).c_str());
      break;
    case 3:
      ioIndicate(Interrupt3);
      esp_restart();
      break;
    default:
      break;
  }


  portENTER_CRITICAL(&interruptMux);
  buttonsPressed[index] = false;
  portEXIT_CRITICAL(&interruptMux);
}


void processInterrupts(){
  for (int i = 0; i < 4; i++){
    if (buttonsPressed[i])
      ProcessInterrupt(i);
  }
}

void bindInterrupts(bool stateIsConfig){
  log_v("Binding interrupts.");

  isConfigState = stateIsConfig;

  pinMode(buttonsPins[2], INPUT_PULLDOWN);
  attachInterrupt(buttonsPins[2], Button2Interrupt, RISING);
  pinMode(buttonsPins[3], INPUT_PULLDOWN);
  attachInterrupt(buttonsPins[3], Button3Interrupt, RISING);

}

void unbindInterrupts() {
  detachInterrupt(buttonsPins[2]);
  detachInterrupt(buttonsPins[3]);
}

void rebindInterrupts() {
  attachInterrupt(buttonsPins[2], Button2Interrupt, RISING);
  attachInterrupt(buttonsPins[3], Button3Interrupt, RISING);
}

// actions on interrupts

void SendLetterAboutButtonPressed(){
  bool result = false;
  // ioIndicate(WAIT);
  ioWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, "Sending test letter");
  result = sendLetter("Message from ESP", "<h1>Hey, your button was pressed!</h1>", true);
  if (result){
    ioIndicate(SUCCESS);
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Sent successfully.");
  } else {
    ioIndicate(ERROR);
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Not sent.");
  }
}

void ListSMTPSettings(){
  char *buffer = (char *)malloc(STRING_LENGTH);
  ioWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, "Smtp settings:");
  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++){
    if (i != EMAIL_PASS && i != EMAIL_IMAP_PORT && i != EMAIL_SMTP_PORT)
      ioWrite(IO_WRITE_SCREEN, getEmailValueFromMemory(i, buffer));
  }
  free(buffer);
}

void ListWiFiFromMemory(){
  int counter = getWiFiCredentialsAmountFromMemory();

  if (counter == 0){
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    ioWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

    for (int i = 0; i < counter; i++){
      getWiFiSsidFromMemory(i, ssid);
      getWiFiPasswordFromMemory(i, password);
      String output = String(i + 1) + ":'" + String(ssid) + "'-'" + String(password) + "'";
      if (i < MAX_LINES_AMOUNT - 1){
        ioWrite(IO_WRITE_SCREEN, output.c_str());
      }
    }

    free(ssid);
    free(password);
  }
}
