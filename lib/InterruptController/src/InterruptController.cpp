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

  if (buttonsPressed[0]){
    IOIndicate(Interrupt0);
    portENTER_CRITICAL(&interruptMux);
    buttonsPressed[0] = false;
    portEXIT_CRITICAL(&interruptMux);
  }
  if (buttonsPressed[1]){
    IOIndicate(Interrupt1);
    portENTER_CRITICAL(&interruptMux);
    buttonsPressed[1] = false;
    portEXIT_CRITICAL(&interruptMux);
  }
  if (buttonsPressed[2]){
    IOIndicate(Interrupt2);
    portENTER_CRITICAL(&interruptMux);
    buttonsPressed[2] = false;
    portEXIT_CRITICAL(&interruptMux);
  }
  if (buttonsPressed[3]){
    IOIndicate(Interrupt3);
    SetStateInMemory(!isConfigState);
    log_v("Switched to %s after restart.", (isConfigState ? "work" : "debug"));
    portENTER_CRITICAL(&interruptMux);
    buttonsPressed[3] = false;
    portEXIT_CRITICAL(&interruptMux);
  }
}


