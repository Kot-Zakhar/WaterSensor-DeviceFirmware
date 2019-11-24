#include <BtTerminalController.h>


BluetoothSerial BT;

const char* bt_credentials_name = "ESP32";

// exceptions
#define NOT_AVAILABLE -2
#define NOT_RECOGNISED -1
// commands
#define BT_PING 0
#define WIFI_SSID_AND_PASSWORD 1
#define WIFI_ERASE_MEMORY 2
#define WIFI_SHOW_NETWORKS 3
#define SMTP_SETTINGS 4
#define RESTART 5
#define SWITCH_MODE 6
#define HELP 7
#define COMMAND_AMOUNT 8

static const char* commands[] = {
  "ping",
  "new wifi",
  "clear",
  "show",
  "smtp",
  "restart",
  "switch mode",
  "help"
};

// bt messages
static const char wifi_ssid_request_message[] = "Provide ssid of network:";
static const char wifi_ssid_confirmation_message[] = "Ssid received.";
static const char wifi_password_request_message[] = "Provide passowrd of network:";
static const char wifi_password_confirmation_message[] = "Password received.";
static const char erasing_memory_message[] = "Erasing all memory";
static const char status_ok_message[] = "Ok";
static const char status_error_message[] = "Error";
static const char unknown_command_message[] = "Unknown command";
static const char restart_message[] = "Restarting...";
static const char pong_message[] = "pong";
static const char memory_empty_message[] = "No networks in memory.";


void PingCommand();
void AddWiFiCredentialsCommand();
void ClearMemoryCommand();
void PrintNetworksFromMemoryCommand();
void SmtpConfigureCommand();
void RestartESPCommand();
void SwitchModeCommand();
void HelpCommand();

void WriteBtLine(const char* line);
int ReadBtLine(char *buffer, int maxLength);
int ReadBtCommand(char *buffer, int maxLength);
int AwaitAndReadBtLine(char *buffer, int maxLength);

void InitBtTerminalController(){
  Serial.println("You can pair to " + String(bt_credentials_name));
  BT.begin(bt_credentials_name);
}

void ProcessBt(){
  if (!BT.available())
    return;

  char *commandLine = (char *)malloc(STRING_LENGTH);

  int command = ReadBtCommand(commandLine, STRING_LENGTH);

  Serial.println("Got command: " + String(command));

  switch (command)
  {
  case BT_PING:
    PingCommand();
    break;
  case WIFI_SSID_AND_PASSWORD:
    AddWiFiCredentialsCommand();
    break;
  case WIFI_ERASE_MEMORY:
    ClearMemoryCommand();
    break;
  case WIFI_SHOW_NETWORKS:
    PrintNetworksFromMemoryCommand();
    break;
  case SMTP_SETTINGS:
    SmtpConfigureCommand();
    break;
  case RESTART:
    RestartESPCommand();
    break;
  case SWITCH_MODE:
    SwitchModeCommand();
    break;
  case HELP:
    HelpCommand();
    break;
  case NOT_RECOGNISED:
  default:
    WriteBtLine(unknown_command_message);
    break;
  }

  free(commandLine);
}

void PingCommand(){
  WriteBtLine(pong_message);
}

void AddWiFiCredentialsCommand(){
  char *ssid = (char *)malloc(STRING_LENGTH);
  char *password = (char *)malloc(STRING_LENGTH);
    
  WriteBtLine(wifi_ssid_request_message);
  if (AwaitAndReadBtLine(ssid, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  Serial.println(wifi_ssid_confirmation_message);

  WriteBtLine(wifi_password_request_message);
  if (AwaitAndReadBtLine(password, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  Serial.println(wifi_password_confirmation_message);

  SaveWiFiCredentialsInMemory(ssid, password);

  WriteBtLine(status_ok_message);

  free(ssid);
  free(password);
}

void PrintNetworksFromMemoryCommand(){
  int counter = GetWiFiCredentialsAmountFromMemory();

  if (counter == 0){
      WriteBtLine(memory_empty_message);
  } else {
    char* ssid = (char*) malloc(STRING_LENGTH * sizeof(char));
    char* password = (char*) malloc(STRING_LENGTH * sizeof(char));

    for (int i = 0; i < counter; i++){
      GetWiFiSsidFromMemory(i, ssid);
      GetWiFiPasswordFromMemory(i, password);
      String output = String(i + 1) + ": '" + String(ssid) + "' - '" + String(password) + "'";
      WriteBtLine(output.c_str());
    }

    free(ssid);
    free(password);
  }
}

void SmtpConfigureCommand(){
  WriteBtLine("Configuring smtp.");
  // todo: Unbind hardcoded index of command to smtp value index
  WriteBtLine((String(
    "0 - exit;\n") + 
    "1 - show all settings;\n" +
    "2 [server] - set server;\n" +
    "3 [port] - set port;\n" +
    "4 [login] - set login;\n" +
    "5 [password] - set pass;\n" +
    "6 [sender name] - set sender name;\n" +
    "7 [recipient email] - set recipient."
  ).c_str());

  String commandLine;
  int command;
  String response;
  char *buffer = (char *)malloc(STRING_LENGTH);

  do {
    WriteBtLine("Write a command in a following way:\n[command number] [value]");    
    if (AwaitAndReadBtLine(buffer, STRING_LENGTH) < 0){
      WriteBtLine(status_error_message);
      return;
    }

    commandLine = String(buffer);
    command = commandLine.toInt();
    commandLine = commandLine.substring(2);
    Serial.println("Command: " + String(command));
    Serial.println("Value: " + String(commandLine));
    switch (command){
      case 0:
        response = String("Exiting.");
        break;
      case 1:
        response = String("Smtp settings:");
        for (int i = 0; i < SMTP_SETTINGS_COUNT; i++){
          // todo: check for setting existance
          response = response + "\n" + smtp_settings[i] + ": " + GetSmtpValue(i, buffer);
        }
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
        SetSmtpValue(SMTP_SERVER + command - 2, commandLine.c_str());
        response = String(status_ok_message);
        break;
      default:
        response = String("Command not recognize.");
    }
    WriteBtLine(response.c_str());
  } while (command != 0);
  free(buffer);
}

void ClearMemoryCommand(){
  WriteBtLine(erasing_memory_message);
  ClearMemory();
}

void RestartESPCommand(){
  WriteBtLine(restart_message);
  ESP.restart();
}

void SwitchModeCommand(){
  bool mode = IsConfigStateInMemory();
  WriteBtLine((String("Current mode is ") + (mode ? "`configuration`" : "`working`")).c_str());

  String answer;
  char *buffer = (char *)malloc(STRING_LENGTH);
  do {
    WriteBtLine((String("Would you like to switch it to ") + (!mode ? "`configuration`" : "`working`") + "? (y/n)").c_str());
    AwaitAndReadBtLine(buffer, STRING_LENGTH);
    answer = String(buffer);
    answer.toLowerCase();
  } while (answer.compareTo("y") && answer.compareTo("n"));
  if (!answer.compareTo("y")){
    SetStateInMemory(!mode);
    WriteBtLine(status_ok_message);
  } else {
    WriteBtLine("Aborting.");
  }
  free(buffer);
}

void HelpCommand(){
  WriteBtLine("List of commands:");
  String message;
  for (int i = 0; i < COMMAND_AMOUNT; i++){
    message += String(commands[i]) + "\n";
  }
  WriteBtLine(message.c_str());
}

// IO

int ReadBtCommand(char *buffer, int maxLength){
  if (!BT.available())
      return NOT_AVAILABLE;
  
  ReadBtLine(buffer, maxLength);
  
  String line = String(buffer);

  for (int j = 0; j < COMMAND_AMOUNT; j++){
    if (!line.compareTo(String(commands[j])))
      return j;
  }
  
  return NOT_RECOGNISED;
}

int AwaitAndReadBtLine(char *buffer, int maxLength){
  while (!BT.available()){}
  return ReadBtLine(buffer, maxLength);
}

int ReadBtLine(char *buffer, int maxLength){
  if (!BT.available())
      return -1;

  int charAmount = BT.readBytes(buffer, maxLength);

  
  for (int i = 0; i < charAmount; i++){
    if ((buffer[i] == '\n') || (buffer[i] == '\r')){
      buffer[i] = 0;
      charAmount = i;
      break;
    }
  }

  return charAmount;
}

void WriteBtLine(const char* line){
  BT.write((const uint8_t *)line, strlen(line));
  BT.write('\n');
  Serial.println(line);
  delay(100);
}