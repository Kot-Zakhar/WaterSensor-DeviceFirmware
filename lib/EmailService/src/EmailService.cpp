#include <EmailService.h>
#include <MemoryController.h>
#include <ESP32_MailClient.h>
#include <WifiController.h>
#include <IOController.h>


#define CHECK_INBOX_PERIOD_S 30

SMTPData smtpData;
IMAPData imapData;

const char* htmlLeftSide = "<br><p>";
const char* htmlSise = "</p><br>";

const String searchCriteria = "UID SEARCH SUBJECT \"esp32\" TEXT \"check\" UNSEEN";


bool sendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess){
  log_v("Sending letter");
  if (!emailServerSettingsAvailable(SMTP_EMAIL_SERVER_TYPE)) {
    log_v("Not all email settings are set!");
    return false;
  }
  if (!isWiFiConnected()) {
    log_v("Wifi is not connected.");
    return false;
  }

  // ioIndicate(EMAIL_SENDING_LETTER);

  struct EmailServerSettings smtpServer;
  getEmailServerSettingsFromMemory(SMTP_EMAIL_SERVER_TYPE, smtpServer);
  syncTime();
  char *dateTime = getDateTimeStr((char *)malloc(STRING_LENGTH), STRING_LENGTH, false);
  // TODO: make this customasable
  char *sender = "ESP32 water sensor";
  
  String timeStampedMessage = 
    String(message) +
    (isHtml ? htmlLeftSide : "\n") + 
    (isHtml ? htmlSise: "\n") + 
    (isHtml ? htmlLeftSide : "\n") + 
    dateTime +
    (isHtml ? htmlSise: "\n");

  log_v("Email:\n ---\n Smtp server: %s\n Smtp port: %s\n Smtp login: %s\n Smtp sender name: %s\n ---\n Subject: %s\n Message:\n %s\n ---\n",
    smtpServer.server,
    smtpServer.port,
    smtpServer.login,
    sender,
    subject,
    timeStampedMessage.c_str()
  );

  smtpData.setDebug(true);

  smtpData.setLogin(smtpServer.server, String(smtpServer.port).toInt(), smtpServer.login, smtpServer.password);
  smtpData.setSender(sender, smtpServer.login);
  smtpData.setPriority("High");
  smtpData.setSubject(subject);
  smtpData.setMessage(timeStampedMessage, isHtml);

  char *recipient = (char *)malloc(STRING_LENGTH);
  int recipientCount = getEmailRecipientsAmountFromMemory();

  for (int i = 0; i < recipientCount; i++) {
    recipient = getEmailRecipientFromMemory(i, recipient, STRING_LENGTH);
    log_d("adding recipient %s", recipient);
    smtpData.addRecipient(recipient);
  }

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

  free(recipient);

  return result;
}

bool checkForIncomingLetter(){
  log_i("Checking incomming letter");
  if (!emailServerSettingsAvailable(IMAP_EMAIL_SERVER_TYPE)){
    log_v("Not all email settings are set!");
    return false;
  }
  bool result = false;

  ioIndicate(EMAIL_CHECKING);

  struct EmailServerSettings imapServer;

  getEmailServerSettingsFromMemory(IMAP_EMAIL_SERVER_TYPE, imapServer);

  imapData.setLogin(imapServer.server, String(imapServer.port).toInt(), imapServer.login, imapServer.password);
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

  return result;
}
