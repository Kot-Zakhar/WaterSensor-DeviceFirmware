#include "InterruptController.h"

const uint8_t ResetButtonPin = 23;
const uint8_t ModePin = 22;
// const uint8_t VOutPin5v = 21;
const uint8_t MessagePin = 19;

static const char *TAG = "InterruptController";

bool restartInterruptQueued = false;
bool changeModeInterruptQueued = false;
bool messageInterruptQueued = false;



void IRAM_ATTR RestartInterrupt() {
  if (!restartInterruptQueued){
    restartInterruptQueued = true;
    ESP_LOGV(TAG, "Restart interrupt");
  }
}

void IRAM_ATTR ChangeModeInterrupt(){
  if (!changeModeInterruptQueued){
    changeModeInterruptQueued = true;
    ESP_LOGV(TAG, "ChangeMode required.");
  }
}

void IRAM_ATTR MessageInterrupt(){
  if (!messageInterruptQueued){
    messageInterruptQueued = true;
    ESP_LOGV(TAG, "Message interrupt.");
  }
}

void BindInterrupts(bool stateIsConfig){
  ESP_LOGV(TAG, "Binding interrupts.");

  // pinMode(VOutPin5v, OUTPUT);
  // digitalWrite(VOutPin5v, HIGH);

  if (!stateIsConfig){
    pinMode(MessagePin, INPUT_PULLUP);
    attachInterrupt(MessagePin, MessageInterrupt, FALLING);

  }

  pinMode(ResetButtonPin, INPUT_PULLUP);
  attachInterrupt(ResetButtonPin, RestartInterrupt, FALLING);

  pinMode(ModePin, INPUT_PULLUP);
  attachInterrupt(ModePin, ChangeModeInterrupt, FALLING);

}


void ProcessMessageInterrupt(){
  if (messageInterruptQueued){
    SendLetter(
      "Кнопка нажата",
      "<h1>Кнопка</h1><p>Кнопка была нажата.</p>",
      true,
      true
    );
    ESP_LOGV(TAG, "Letter interrupt processed.");
    messageInterruptQueued = false;
    delay(500);
  }
}

void ProcessChangeModeInterrupt(bool stateIsConfig){
  if (changeModeInterruptQueued){
    SetStateInMemory(!stateIsConfig);
    ESP_LOGV(TAG, "Switched to %s after restart.", stateIsConfig ? "work" : "debug");
    changeModeInterruptQueued = false;
    delay(500);
  }
}

void ProcessRestartInterrupt(){
  if (restartInterruptQueued){
    ESP_LOGV(TAG, "Restart interrupt processing.");
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


