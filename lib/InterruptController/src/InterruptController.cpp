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
  portENTER_CRITICAL_ISR(&interruptMux);
  buttonsPressed[0] = true;
  portEXIT_CRITICAL_ISR(&interruptMux);
  delay(100);
}

void IRAM_ATTR Button1Interrupt(){
  portENTER_CRITICAL_ISR(&interruptMux);
  buttonsPressed[1] = true;
  portEXIT_CRITICAL_ISR(&interruptMux);
  delay(100);
}

void IRAM_ATTR Button2Interrupt(){
  portENTER_CRITICAL_ISR(&interruptMux);
  buttonsPressed[2] = true;
  portEXIT_CRITICAL_ISR(&interruptMux);
  delay(100);
}

void IRAM_ATTR Button3Interrupt(){
  portENTER_CRITICAL_ISR(&interruptMux);
  buttonsPressed[3] = true;
  portEXIT_CRITICAL_ISR(&interruptMux);
  delay(100);
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


void ProcessInterrupts(){

  portENTER_CRITICAL(&interruptMux);
  if (buttonsPressed[0]){
    IOIndicate(Interrupt0);
    buttonsPressed[0] = false;
  }
  if (buttonsPressed[1]){
    IOIndicate(Interrupt1);
    buttonsPressed[1] = false;
  }
  if (buttonsPressed[2]){
    IOIndicate(Interrupt2);
    buttonsPressed[2] = false;
  }
  if (buttonsPressed[3]){
    IOIndicate(Interrupt3);
    buttonsPressed[3] = false;
  }
  portEXIT_CRITICAL(&interruptMux);
}


