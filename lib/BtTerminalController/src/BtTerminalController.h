#pragma once
#ifndef _BLUETOOTH_TERMINAL_CONTROLLER_H_
#define _BLUETOOTH_TERMINAL_CONTROLLER_H_

#include <BtHelper.h>
#include <Preferences.h>

#define STRING_LENGTH 100

// memory keys
extern const char* wifi_table_name;
extern const char* wifi_ssid_key_prefix;
extern const char* wifi_password_key_prefix;
extern const char* wifi_amount_key;


void BtControllerSetup(BluetoothSerial *SerialBT, Preferences *preferences);

void ProcessBt();
void AddCredentialsCommand();
int SaveCredentials(const char* ssid, const char* password);
void PrintNetworksFromMemory(Preferences *memory, BluetoothSerial *SerialBT);
void ClearMemoryCommand();
void RestartESPCommand();

#endif