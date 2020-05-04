#ifndef BLUETOOTH_JSON_H
#define BLUETOOTH_JSON_H

#include <ArduinoJson.h>
#include <BtIO.h>
#include <BtCommands.h>
#include <MemoryController.h>

void ProcessBtJsonCommand(int command);

// void PingJsonCommand();
// void AddWiFiCredentialsJsonCommand();
// void RemoveAllWiFiCredentialsJsonCommand();
// void PrintNetworksFromMemoryJsonCommand();
// void SmtpConfigureJsonCommand();
// void RestartESPJsonCommand();
// void SwitchModeJsonCommand();
// void HelpJsonCommand();

#endif