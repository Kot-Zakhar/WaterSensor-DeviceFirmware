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
  16,
  0,
  14,
  2,
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

  switch(index){
    case 0:
      IOIndicate(Interrupt0);
      if (isConfigState){
        ListSMTPSettings();
      } else {
        SendLetterAboutButtonPressed();
      }
      break;
    case 1:
      IOIndicate(Interrupt1);
      ListWiFiFromMemory();
      break;
    case 2:
      break;
    case 3:
      IOIndicate(Interrupt3);
      delay(100);
      SetStateInMemory(!isConfigState);
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, (String("After restart: ") + (isConfigState ? "work" : "config")).c_str());
      break;
    default:
      break;
  }


  portENTER_CRITICAL(&interruptMux);
  buttonsPressed[index] = false;
  portEXIT_CRITICAL(&interruptMux);
}


void ProcessInterrupts(){
  for (int i = 0; i < 4; i++){
    if (buttonsPressed[i])
      ProcessInterrupt(i);
  }
}

void BindInterrupts(bool stateIsConfig){
  log_v("Binding interrupts.");

  isConfigState = stateIsConfig;

  pinMode(buttonsPins[0], INPUT_PULLDOWN);
  attachInterrupt(buttonsPins[0], Button0Interrupt, RISING);
  pinMode(buttonsPins[1], INPUT_PULLDOWN);
  attachInterrupt(buttonsPins[1], Button1Interrupt, RISING);
  pinMode(buttonsPins[2], INPUT_PULLDOWN);
  attachInterrupt(buttonsPins[2], Button2Interrupt, RISING);
  pinMode(buttonsPins[3], INPUT_PULLDOWN);
  attachInterrupt(buttonsPins[3], Button3Interrupt, RISING);

}


// actions on interrupts

void SendLetterAboutButtonPressed(){
  bool result = false;
  IOIndicate(WAIT);
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, "Sending letter...");
  result = SendLetter("Message from ESP", "<h1>Hey, your button was pressed!</h1>", true);
  if (result){
    IOIndicate(SUCCESS);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Sent successfully.");
  } else {
    IOIndicate(ERROR);
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Not sent.");
  }
}

void ListSMTPSettings(){
  char *buffer = (char *)malloc(STRING_LENGTH);
  IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE, "Smtp settings:");
  for (int i = 0; i < EMAIL_SETTINGS_COUNT; i++){
    if (i != EMAIL_PASS && i != EMAIL_IMAP_PORT && i != EMAIL_SMTP_PORT)
      IOWrite(IO_WRITE_SCREEN, GetEmailValue(i, buffer));
  }
  free(buffer);
}

void ListWiFiFromMemory(){
  int counter = GetWiFiCredentialsAmountFromMemory();

  if (counter == 0){
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    IOWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

    for (int i = 0; i < counter; i++){
      GetWiFiSsidFromMemory(i, ssid);
      GetWiFiPasswordFromMemory(i, password);
      String output = String(i + 1) + ":'" + String(ssid) + "'-'" + String(password) + "'";
      if (i < MAX_LINES_AMOUNT - 1){
        IOWrite(IO_WRITE_SCREEN, output.c_str());
      }
    }

    free(ssid);
    free(password);
  }
}
