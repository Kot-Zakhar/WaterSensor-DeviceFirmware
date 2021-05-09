#include "EmailChecker.h"
#include <EmailService.h>
#include <GsmService.h>

#include <Ticker.h>


// Ticker emailChecker;
bool needToCheckInbox = false;

void checkLetterTimerEvent() {
  // log_v("Check letter event.");
  needToCheckInbox = true;
}

void startEmailChecker() {
  log_v("Initializing email checker.");
//   emailChecker.attach(CHECK_INBOX_PERIOD_S, checkLetterTimerEvent);
}

void pauseEmailChecker() {
  log_v("Stopping email checker.");
//   emailChecker.detach();
}

void resumeEmailChecker() {
  log_v("Restarting email checker.");
//   emailChecker.attach(CHECK_INBOX_PERIOD_S, checkLetterTimerEvent);
}

bool emailNeedToBeProcessed() {
  return needToCheckInbox;
}

void processEmailChecker() {
  if (needToCheckInbox){
    // checkForIncomingLetter();
    needToCheckInbox = false;
  }
}