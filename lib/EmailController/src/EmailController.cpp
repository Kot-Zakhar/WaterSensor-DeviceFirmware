#include <EmailController.h>

SMTPData smtpData;
IMAPData imapData;

const char* htmlLeftSide = "<br><p>";
const char* htmlSise = "</p><br>";

const String searchCriteria = "UID SEARCH SUBJECT \"esp32\" TEXT \"check\" UNSEEN";

bool SendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess){
  log_v("Sending letter");
  if (!EmailValuesAvailable()){
    log_v("Not all email settings are set!");
    return false;
  }
  if (!IsWiFiConnected()){
    log_v("Wifi is not connected.");
    return false;
  }
  char *server = GetEmailValue(EMAIL_SMTP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = GetEmailValue(EMAIL_SMTP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = GetEmailValue(EMAIL_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = GetEmailValue(EMAIL_PASS, (char *)malloc(STRING_LENGTH));
  char *sender = GetEmailValue(EMAIL_SMTP_SENDER, (char *)malloc(STRING_LENGTH));
  char *recipient = GetEmailValue(EMAIL_SMTP_RECIPIENT, (char *)malloc(STRING_LENGTH));
  char *dateTime = GetDateTimeStr((char *)malloc(STRING_LENGTH), STRING_LENGTH, false);
  
  String timeStampedMessage = 
    String(message) +
    (isHtml ? htmlLeftSide : "\n") + 
    "Sensor value is: " + GetSensorValue() +
    (isHtml ? htmlSise: "\n") + 
    (isHtml ? htmlLeftSide : "\n") + 
    dateTime +
    (isHtml ? htmlSise: "\n");

  log_v("Email:\n ---\n Smtp server: %s\n Smtp port: %s\n Smtp login: %s\n Smtp sender name: %s\n ---\n Recipient: %s\n Subject: %s\n Message:\n %s\n ---\n",
    server,
    port,
    login,
    sender,
    recipient,
    subject,
    timeStampedMessage.c_str()
  );

  smtpData.setDebug(true);

  smtpData.setLogin(server, String(port).toInt(), login, password);
  smtpData.setSender(sender, login);
  smtpData.setPriority("High");
  smtpData.setSubject(subject);
  smtpData.setMessage(timeStampedMessage, isHtml);
  smtpData.addRecipient(recipient);

  bool result = false;
  do {
    log_v("Trying to send email..." );
    result = MailClient.sendMail(smtpData);
    if (result)
      log_v("Successfully sent.");
    else
      log_v("Not sent.");
  } while(!result && retryUntilSuccess);

  smtpData.empty();

  free(server);
  free(port);
  free(login);
  free(password);
  free(sender);
  free(recipient);

  return result;
}

bool CheckForIncomingLetter(){
  log_i("Checking incomming letter");
  if (!EmailValuesAvailable()){
    log_v("Not all email settings are set!");
    return false;
  }
  bool result = false;

  char *server = GetEmailValue(EMAIL_IMAP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = GetEmailValue(EMAIL_IMAP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = GetEmailValue(EMAIL_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = GetEmailValue(EMAIL_PASS, (char *)malloc(STRING_LENGTH));

  imapData.setLogin(server, String(port).toInt(), login, password);
  imapData.setSearchCriteria(searchCriteria);
  imapData.setRecentSort(true);
  imapData.setSearchLimit(1);

  if (MailClient.readMail(imapData) && imapData.availableMessages())
  {
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Got check mail.");
    IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Replying...");
    
    MailClient.setFlag(imapData, imapData.getUID(0).toInt(), "\\Seen");

    String message = String("Got the message. I'm working fine :)\n") +
      "Sensor value is: " + GetSensorValue();
    String subject = "Reply to \"" +
      imapData.getSubject(0) + "\"";

    result = SendLetter(subject.c_str(), message.c_str(), false);
    if (result){
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Replied successfully.");
    } else {
      IOWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Could not reply.");
    }
      
  } else {
    log_i("No check letters in mail detected.");
  }

  free(password);
  free(login);
  free(port);
  free(server);

  return result;
}
