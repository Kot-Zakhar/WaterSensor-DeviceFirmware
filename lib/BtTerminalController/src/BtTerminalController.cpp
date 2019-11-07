#include <BtTerminalController.h>

BluetoothSerial BT;

const char* bt_credentials_name = "ESP32";

#define COMMAND_AMOUNT 7
// exceptions
#define NOT_AVAILABLE -2
#define NOT_RECOGNISED -1
// commands
#define BT_PING 0
#define WIFI_SSID_AND_PASSWORD 1
#define WIFI_ERASE_MEMORY 2
#define WIFI_SHOW_NETWORKS 3
#define RESTART 4
#define HELP 5
#define WIFI_CONNECT 6

// bt messages
const char* wifi_ssid_request_message = "Provide ssid of network:";
const char* wifi_ssid_confirmation_message = "Ssid received.";
const char* wifi_password_request_message = "Provide passowrd of network:";
const char* wifi_password_confirmation_message = "Password received.";
const char* erasing_memory_message = "Erasing all memory";
const char* status_ok_message = "Ok";
const char* status_error_message = "Error";
const char* unknown_command_message = "Unknown command";
const char* restart_message = "Restarting...";
const char* pong_message = "pong";
const char* memory_empty_message = "No networks in memory.";

// bt commands
char* BT_PING_LINE = "ping";
char* WIFI_SSID_AND_PASSWORD_LINE =  "new";
char* WIFI_ERASE_MEMORY_LINE =  "clear";
char* WIFI_SHOW_NETWORKS_LINE = "show";
char* RESTART_LINE = "restart";
char* HELP_LINE = "help";
char* WIFI_CONNECT_LINE = "connect";

char* commands[] = {
    BT_PING_LINE, 
    WIFI_SSID_AND_PASSWORD_LINE, 
    WIFI_ERASE_MEMORY_LINE, 
    WIFI_SHOW_NETWORKS_LINE,
    RESTART_LINE,
    HELP_LINE,
    WIFI_CONNECT_LINE
};

void ProcessBt();
void PingCommand();
void AddCredentialsCommand();
void PrintNetworksFromMemoryCommand();
void ConnectCommand();
void ClearMemoryCommand();
void RestartESPCommand();

void WriteBtLine(const char* line);
int ReadBtLine(String *buffer, int maxLength);
int ReadBtCommand(String *buffer, int maxLength);

void InitBtTerminalController(){
  Serial.println("You can pair to " + String(bt_credentials_name));
  BT.begin(bt_credentials_name);

}

void ProcessBt(){
  if (!BT.available())
    return;

  String commandLine = String(STRING_LENGTH);

  int command = ReadBtCommand(&commandLine, STRING_LENGTH);

  Serial.println("Got command: " + String(command));

  switch (command)
  {
  case BT_PING:
    PingCommand();
    break;
  case WIFI_SSID_AND_PASSWORD:
    AddCredentialsCommand();
    break;
  case WIFI_ERASE_MEMORY:
    ClearMemoryCommand();
    break;
  case WIFI_SHOW_NETWORKS:
    PrintNetworksFromMemoryCommand();
    break;
  case WIFI_CONNECT:
    ConnectCommand();
    break;
  case RESTART:
    RestartESPCommand();
    break;
  case NOT_RECOGNISED:
  default:
    WriteBtLine(unknown_command_message);
    break;
  }
  
}

void AddCredentialsCommand(){
  String ssid = String(STRING_LENGTH);
  String password = String(STRING_LENGTH);
    
  WriteBtLine(wifi_ssid_request_message);
  while (!BT.available()){}
  Serial.println(wifi_ssid_confirmation_message);
  if (ReadBtLine(&ssid, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  WriteBtLine(wifi_password_request_message);
  while(!BT.available()){}
  Serial.println(wifi_password_confirmation_message);
  if (ReadBtLine(&password, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  SaveCredentialsInMemory(ssid.c_str(), password.c_str());
  WriteBtLine(status_ok_message);
}

void PrintNetworksFromMemoryCommand(){
  int counter = GetCredentialsAmountFromMemory();

  if (counter == 0){
      WriteBtLine(memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    for (int i = 0; i < counter; i++){
      GetSsidFromMemory(i, ssid);
      GetPasswordFromMemory(i, password);
      String output = String(i + 1) + ": '" + String(ssid) + "' - '" + String(password) + "'";
      WriteBtLine(output.c_str());
    }

    free(ssid);
    free(password);
  }
}

void ConnectCommand(){
  PrintNetworksFromMemoryCommand();
  

  int amount = GetCredentialsAmountFromMemory();
  String message;
  if (amount == 0){
    message = "Provide credentials by '" + String(WIFI_SSID_AND_PASSWORD_LINE) + "' command.";
    WriteBtLine(message.c_str());
    return;
  }

  String buffer = String(STRING_LENGTH);

  message = "Wich network to connect to?";
  WriteBtLine(message.c_str());
  while (!BT.available()){}
  if (ReadBtLine(&buffer, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }

  int index = buffer.toInt();

  char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
  char* password = (char*) malloc(STRING_LENGTH * sizeof(char));


  if (index <= 0 || index > amount){
    WriteBtLine("Trying to connect to each network...");
    for (int i = 0; i < amount; i++){
      GetSsidFromMemory(i, ssid);
      WriteBtLine((String("Connecting to: ") + ssid).c_str());
      ConnectToWiFi(
        ssid, 
        GetPasswordFromMemory(i, password)
      );
      delay(CONNECTING_TIMEOUT);
      if (IsWiFiConnected()){
        WriteBtLine(status_ok_message);
        break;
      }
    }
  } else {
    ConnectToWiFi(
      GetSsidFromMemory(index - 1, ssid), 
      GetPasswordFromMemory(index - 1, password)
    );
  }

  if (!IsWiFiConnected())
    WriteBtLine("Can't connect.");

  free(ssid);
  free(password);
}

void PingCommand(){
  WriteBtLine(pong_message);
}

void ClearMemoryCommand(){
  WriteBtLine(erasing_memory_message);
  ClearMemory();
}

void RestartESPCommand(){
  WriteBtLine(restart_message);
  ESP.restart();
}




// IO

int ReadBtCommand(String *buffer, int maxLength){
    if (!BT.available())
        return NOT_AVAILABLE;
    
    int symbolsRead = ReadBtLine(buffer, maxLength);
    
    for (int j = 0; j < COMMAND_AMOUNT; j++){
        if (!buffer->compareTo(String(commands[j])))
            return j;
    }
    
    return NOT_RECOGNISED;
}

int ReadBtLine(String *buffer, int maxLength){
    if (!BT.available() || buffer->length() <= 0)
        return -1;

    int index = 0;

    char character = 0;
    buffer->clear();

    while (index < maxLength - 1){
        character = (char)BT.read();
        if ((character == '\n') || (character == '\r'))
            break;
        *buffer = *buffer + character;
        index++;
    } 
    Serial.println(*buffer);

    while (BT.available())
        BT.read();

    return buffer->length();
}

void WriteBtLine(const char* line){
  BT.write((const uint8_t *)line, strlen(line));
  BT.write('\n');
  Serial.println(line);
}