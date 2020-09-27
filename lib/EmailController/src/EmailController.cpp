#include <EmailController.h>

#define CHECK_INBOX_PERIOD_S 30

SMTPData smtpData;
IMAPData imapData;

Ticker emailChecker;
bool needToCheckInbox = false;

const char* htmlLeftSide = "<br><p>";
const char* htmlSise = "</p><br>";

const String searchCriteria = "UID SEARCH SUBJECT \"esp32\" TEXT \"check\" UNSEEN";

void checkLetterTimerEvent(){
  log_v("Check letter event.");
  needToCheckInbox = true;
}

void initEmailController(){
  log_v("Initializing email checker.");
  emailChecker.attach(CHECK_INBOX_PERIOD_S, checkLetterTimerEvent);
}

void stopEmailChecker() {
  log_v("Stopping email checker.");
  emailChecker.detach();
}

void restartEmailChecker() {
  log_v("Restarting email checker.");
  emailChecker.attach(CHECK_INBOX_PERIOD_S, checkLetterTimerEvent);
}

bool sendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess){
  log_v("Sending letter");
  if (!EmailValuesAvailable()) {
    log_v("Not all email settings are set!");
    return false;
  }
  if (!isWiFiConnected()) {
    log_v("Wifi is not connected.");
    return false;
  }

  ioIndicate(EMAIL_SENDING_LETTER);

  char *server = getEmailValue(EMAIL_SMTP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = getEmailValue(EMAIL_SMTP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = getEmailValue(EMAIL_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = getEmailValue(EMAIL_PASS, (char *)malloc(STRING_LENGTH));
  char *sender = getEmailValue(EMAIL_SMTP_SENDER, (char *)malloc(STRING_LENGTH));
  char *recipient = getEmailValue(EMAIL_SMTP_RECIPIENT, (char *)malloc(STRING_LENGTH));
  char *dateTime = getDateTimeStr((char *)malloc(STRING_LENGTH), STRING_LENGTH, false);
  
  String timeStampedMessage = 
    String(message) +
    (isHtml ? htmlLeftSide : "\n") + 
    "Sensor value is: " + getSensorValue() +
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

  if (result) {
    ioIndicate(SUCCESS);
  } else {
    ioIndicate(ERROR);
  }

  smtpData.empty();

  free(server);
  free(port);
  free(login);
  free(password);
  free(sender);
  free(recipient);

  return result;
}

bool checkForIncomingLetter(){
  log_i("Checking incomming letter");
  if (!EmailValuesAvailable()){
    log_v("Not all email settings are set!");
    return false;
  }
  bool result = false;

  ioIndicate(EMAIL_CHECKING);

  char *server = getEmailValue(EMAIL_IMAP_SERVER, (char *)malloc(STRING_LENGTH));
  char *port = getEmailValue(EMAIL_IMAP_PORT, (char *)malloc(STRING_LENGTH));
  char *login = getEmailValue(EMAIL_LOGIN, (char *)malloc(STRING_LENGTH));
  char *password = getEmailValue(EMAIL_PASS, (char *)malloc(STRING_LENGTH));

  imapData.setLogin(server, String(port).toInt(), login, password);
  imapData.setSearchCriteria(searchCriteria);
  imapData.setRecentSort(true);
  imapData.setSearchLimit(1);

  if (MailClient.readMail(imapData) && imapData.availableMessages())
  {
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Got check letter.");
    ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Replying...");
    ioIndicate(EMAIL_MESSAGE_FOUND);
    
    String subject = imapData.getSubject(0);

    log_i("from %s\n subject %s", imapData.getFrom(0), subject);

    MailClient.setFlag(imapData, imapData.getUID(0).toInt(), "\\Seen");

    String message = String("Got the message. I'm working fine :)\n");
    String replySubject = "Reply to \"" + subject + "\"";

    result = sendLetter(replySubject.c_str(), message.c_str(), false);
    if (result){
      ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Replied successfully.");
    } else {
      ioWrite(IO_WRITE_SCREEN | IO_WRITE_SERIAL, "Could not reply.");
    }
      
  } else {
    log_i("No check letters in mail detected.");
    ioIndicate(EMAIL_NO_MESSAGE_FOUND);
  }

  free(password);
  free(login);
  free(port);
  free(server);

  return result;
}

bool shouldEmailBeProcessed() {
  return needToCheckInbox;
}

void processEmailController() {
  if (needToCheckInbox){
    checkForIncomingLetter();
    needToCheckInbox = false;
  }
}