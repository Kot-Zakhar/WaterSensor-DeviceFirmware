#include <InterruptController.h>

#include <custom_types.h>
#include <CommandHandlers.h>

static const char *memory_empty_message = "No networks in memory";


bool isConfigState;

volatile bool buttonsPressed[] = {
  false,
  false
};

volatile uint8_t buttonsPins[] = {
  2,
  14,
};

portMUX_TYPE interruptMux = portMUX_INITIALIZER_UNLOCKED;


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

void switchScreenPage();
void changeState();

void ProcessInterrupt(int index){
  switch(index){
    case 0:
      log_d("Processing 0 button: switching screen page.");
      switchScreenPage();
      break;
    case 1:
      log_d("Processing 1 button: changing state.");
      changeState();
      break;
    default:
      break;
  }


  portENTER_CRITICAL(&interruptMux);
  buttonsPressed[index] = false;
  portEXIT_CRITICAL(&interruptMux);
}


void processButtons(){
  for (int i = 0; i < 2; i++){
    if (buttonsPressed[i])
      ProcessInterrupt(i);
  }
}

void initButtonsInterrupts(bool stateIsConfig){
  log_v("Binding buttons.");

  isConfigState = stateIsConfig;

  pinMode(buttonsPins[0], INPUT_PULLDOWN);
  pinMode(buttonsPins[1], INPUT_PULLDOWN);

  buttonsPressed[0] = false;
  buttonsPressed[1] = false;

  rebindInterrupts();
}

void unbindInterrupts() {
  detachInterrupt(buttonsPins[0]);
  detachInterrupt(buttonsPins[1]);
}

void rebindInterrupts() {
  attachInterrupt(buttonsPins[0], Button0Interrupt, RISING);
  attachInterrupt(buttonsPins[1], Button1Interrupt, RISING);
}


// void ListWiFiFromMemory(){
//   int counter = getWiFiCredentialsAmountFromMemory();

//   if (counter == 0){
//     ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, memory_empty_message);
//   } else {
//     char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
//     char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

//     ioWrite(IO_WRITE_SCREEN | IO_WRITE_CLEAN_BEFORE_WRITE | IO_WRITE_SERIAL, (String(counter) + " networks in memory.").c_str());

//     for (int i = 0; i < counter; i++){
//       getWiFiSsidFromMemory(i, ssid);
//       getWiFiPasswordFromMemory(i, password);
//       String output = String(i + 1) + ":'" + String(ssid) + "'-'" + String(password) + "'";
//       if (i < MAX_LINES_AMOUNT - 1){
//         ioWrite(IO_WRITE_SCREEN, output.c_str());
//       }
//     }

//     free(ssid);
//     free(password);
//   }
// }


void switchScreenPage() {
  showNextScreenPage();
}

void changeState() {
  switchStateToNext();
}