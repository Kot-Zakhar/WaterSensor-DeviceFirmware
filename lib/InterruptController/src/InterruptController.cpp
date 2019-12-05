#include "InterruptController.h"

static const char *interruptTag = "InterruptController";

bool restartInterruptQueued = false;
bool changeModeInterruptQueued = false;
bool messageInterruptQueued = false;



void IRAM_ATTR RestartInterrupt() {
  if (!restartInterruptQueued){
    restartInterruptQueued = true;
    log_v("[%s] %s", interruptTag, "Restart interrupt");
  }
}

void IRAM_ATTR ChangeModeInterrupt(){
  if (!changeModeInterruptQueued){
    changeModeInterruptQueued = true;
    log_v("[%s] %s", interruptTag, "ChangeMode required.");
  }
}

void IRAM_ATTR MessageInterrupt(){
  if (!messageInterruptQueued){
    messageInterruptQueued = true;
    log_v("[%s] %s", interruptTag, "Message interrupt.");
  }
}

void BindInterrupts(bool stateIsConfig){
  log_v("[%s] %s", interruptTag, "Binding interrupts.");

  // pinMode(VOutPin5v, OUTPUT);
  // digitalWrite(VOutPin5v, HIGH);

  if (!stateIsConfig){
    pinMode(MESSAGE_BUTTON_PIN, INPUT);
    attachInterrupt(MESSAGE_BUTTON_PIN, MessageInterrupt, RISING);

  }

  pinMode(RESET_BUTTON_PIN, INPUT);
  attachInterrupt(RESET_BUTTON_PIN, RestartInterrupt, RISING);

  pinMode(MODE_BUTTON_PIN, INPUT);
  attachInterrupt(MODE_BUTTON_PIN, ChangeModeInterrupt, RISING);

}


void ProcessMessageInterrupt(){
  if (messageInterruptQueued){
    SendLetter(
      "Кнопка нажата",
      "<h1>Кнопка</h1><p>Кнопка была нажата.</p>",
      true,
      true
    );
    log_v("[%s] %s", interruptTag, "Letter interrupt processed.");
    messageInterruptQueued = false;
    delay(500);
  }
}

void ProcessChangeModeInterrupt(bool stateIsConfig){
  if (changeModeInterruptQueued){
    SetStateInMemory(!stateIsConfig);
    log_v("[%s] %s", interruptTag, "Switched to %s after restart.", stateIsConfig ? "work" : "debug");
    changeModeInterruptQueued = false;
    delay(500);
  }
}

void ProcessRestartInterrupt(){
  if (restartInterruptQueued){
    log_v("[%s] %s", interruptTag, "Restart interrupt processing.");
    ESP.restart();
  }
}



void ProcessInterrupts(bool stateIsConfig){
  if (!stateIsConfig){
    ProcessMessageInterrupt();
  }
  ProcessChangeModeInterrupt(stateIsConfig);
  ProcessRestartInterrupt();
}


