#ifndef _EMAIL_CONTROLLER_H_
#define _EMAIL_CONTROLLER_H_

bool sendLetter(const char *subject, const char *message, bool isHtml, bool retryUntilSuccess = false);

bool checkForIncomingLetter();

#endif