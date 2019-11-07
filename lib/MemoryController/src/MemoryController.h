#pragma once
#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

#include <Preferences.h>

#define STRING_LENGTH 100

// memory keys
extern const char* wifi_table_name;
extern const char* wifi_ssid_key_prefix;
extern const char* wifi_password_key_prefix;
extern const char* wifi_amount_key;

void InitMemoryController();

int SaveCredentialsInMemory(const char* ssid, const char* password);

void RemoveCredentialsFromMemory(int index);
void RemoveCredentialsFromMemory(const char* ssid);
void ClearMemory();

int GetCredentialsAmountFromMemory();
char* GetSsidFromMemory(int index, char* buffer);
char* GetPasswordFromMemory(int index, char* buffer);

#endif