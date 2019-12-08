#include "InterruptController.h"

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
  bool result = false;

  switch(index){
    case 0:
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
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      IOIndicate(Interrupt3);
      SetStateInMemory(!isConfigState);
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, (String("Switching to ") + (isConfigState ? "work" : "debug")).c_str());
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "after restart.");
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
