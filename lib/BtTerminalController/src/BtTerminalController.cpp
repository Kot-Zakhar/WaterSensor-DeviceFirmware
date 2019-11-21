#include <BtTerminalController.h>


BluetoothSerial BT;

const char* bt_credentials_name = "ESP32";

#define COMMAND_AMOUNT 8
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
#define WIFI_SEND_EMAIL 7

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

// bt commands
static char BT_PING_LINE[] = "ping";
static char WIFI_SSID_AND_PASSWORD_LINE[] =  "new";
static char WIFI_ERASE_MEMORY_LINE[] =  "clear";
static char WIFI_SHOW_NETWORKS_LINE[] = "show";
static char RESTART_LINE[] = "restart";
static char HELP_LINE[] = "help";
static char WIFI_CONNECT_LINE[] = "connect";
static char WIFI_SEND_EMAIL_LINE[] = "email";

static const char* commands[] = {
  BT_PING_LINE,
  WIFI_SSID_AND_PASSWORD_LINE, 
  WIFI_ERASE_MEMORY_LINE, 
  WIFI_SHOW_NETWORKS_LINE,
  RESTART_LINE,
  HELP_LINE,
  WIFI_CONNECT_LINE,
  WIFI_SEND_EMAIL_LINE
};

void ProcessBt();

void PingCommand();
void AddCredentialsCommand();
void PrintNetworksFromMemoryCommand();
void ConnectCommand();
void ClearMemoryCommand();
void RestartESPCommand();
void HelpCommand();
void SendEmailCommand();

void WriteBtLine(const char* line);
int ReadBtLine(String *buffer, int maxLength);
int ReadBtCommand(String *buffer, int maxLength);
int AwaitAndReadBtLine(String *buffer, int maxLength);

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
  case WIFI_SEND_EMAIL:
    SendEmailCommand();
    break;
  case HELP:
    HelpCommand();
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
  if (AwaitAndReadBtLine(&ssid, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  Serial.println(wifi_ssid_confirmation_message);

  WriteBtLine(wifi_password_request_message);
  if (AwaitAndReadBtLine(&password, STRING_LENGTH) < 0){
    WriteBtLine(status_error_message);
    return;
  }
  Serial.println(wifi_password_confirmation_message);

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
  if (AwaitAndReadBtLine(&buffer, STRING_LENGTH) < 0){
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
    }
  } else {
    ConnectToWiFi(
      GetSsidFromMemory(index - 1, ssid), 
      GetPasswordFromMemory(index - 1, password)
    );
    delay(CONNECTING_TIMEOUT);
  }

  if (!IsWiFiConnected()){
    WriteBtLine("Can't connect.");
    DisconnectFromWiFi();
  }
  else
    WriteBtLine("Connected.");

  free(ssid);
  free(password);
}


void PingCommand(){
  WriteBtLine(pong_message);
    
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
}

void ClearMemoryCommand(){
  WriteBtLine(erasing_memory_message);
  ClearMemory();
}

void RestartESPCommand(){
  WriteBtLine(restart_message);
  ESP.restart();
}

void HelpCommand(){
  WriteBtLine("List of commands:");
  String message;
  for (int i = 0; i < COMMAND_AMOUNT; i++){
    message += String(commands[i]) + "\n";
  }
  WriteBtLine(message.c_str());
}

void SendEmailCommand(){
  WriteBtLine("You are about to write and email.");
  WriteBtLine((String(
    "1 [line] - append message;\n") +
    "2 - reset last line;\n" + 
    "3 - reset all message;\n" +
    "4 [email] - set recipient;\n" +
    "5 [y/N] - toggle html format (false by default);\n" +
    "6 [subject] - set subject;\n" +
    "7 - commit;\n" +
    "100 - abort."
  ).c_str());

  String commandLine = String(STRING_LENGTH);
  int command;
  String defaultMessage = String("<div style=\"color:#ff0000;font-size:20px;\">Hello World! - From ESP32</div>");
  String subject = String("Message from ESP32");
  String message = String();
  String recipient = String("kot.zakhar@gmail.com");
  String email = String("kot.zakhar@gmail.com");
  bool isHtml = true;


  do {
    WriteBtLine("Write a command in a following way:\n[command number] [value]");    
    if (AwaitAndReadBtLine(&commandLine, STRING_LENGTH) < 0){
      WriteBtLine(status_error_message);
      return;
    }

    command = commandLine.toInt();
    commandLine = commandLine.substring(2);
    Serial.println("Command: " + String(command));
    Serial.println("Value: " + String(commandLine));
    switch (command){
      case 1:
        if (message.isEmpty())
          isHtml = false;
        message += commandLine + '\n';
        WriteBtLine(message.c_str());
        break;
      case 2:
        message.setCharAt(message.lastIndexOf('\n'), 0);
        WriteBtLine(message.c_str());
        break;
      case 3:
        message.clear();
        isHtml = true;
        break;
      case 4:
        recipient = commandLine;
        break;
      case 5:
        commandLine.trim();
        commandLine.toLowerCase();
        isHtml = !commandLine.compareTo("y");
        WriteBtLine(String("Html format is set to ") + isHtml ? "true" : "false");
        break;
      case 6:
        commandLine.trim();
        subject = commandLine;
        break;
      case 7:
        WriteBtLine("Sending email...");
        delay(500);
        SendLetter(
          email,
          subject,
          message,
          false
        );
        delay(500);
        // if (SendLetter(recipient, subject, message.isEmpty() ? defaultMessage : message, isHtml)){
        //   WriteBtLine("Email sent successfully.");
        // } else {
        //   WriteBtLine("Couldn't send email. Abort...");
        // }
        break;
      case 100:
        WriteBtLine("Aborting...");
        break;
      case 0:
      default:
        WriteBtLine("Command not recognize.");
    }
  } while (command != 7 && command != 100);
}



// IO

int ReadBtCommand(String *buffer, int maxLength){
    if (!BT.available())
        return NOT_AVAILABLE;
    
    ReadBtLine(buffer, maxLength);
    
    for (int j = 0; j < COMMAND_AMOUNT; j++){
        if (!buffer->compareTo(String(commands[j])))
            return j;
    }
    
    return NOT_RECOGNISED;
}

int AwaitAndReadBtLine(String *buffer, int maxLength){
  while (!BT.available()){}
  return ReadBtLine(buffer, maxLength);
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

    while (BT.available()){
      BT.read();
    }

    return buffer->length();
}

void WriteBtLine(const char* line){
  BT.write((const uint8_t *)line, strlen(line));
  BT.write('\n');
  Serial.println(line);
  delay(100);
}