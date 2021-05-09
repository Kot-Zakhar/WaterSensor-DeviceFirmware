#ifndef _EMAIL_CHECKER_H_
#define _EMAIL_CHECKER_H_

void startEmailChecker();

void pauseEmailChecker();
void resumeEmailChecker();

bool emailNeedToBeProcessed();
void processEmailChecker();

#endif