#ifndef _NOTIFICATION_SERVICE_H_
#define _NOTIFICATION_SERVICE_H_

enum notification_event_t {
    TEST_SMS_NOTIFICATION,
    TEST_EMAIL_GPRS_NOTIFICATION,
    TEST_EMAIL_WIFI_NOTIFICATION,
    SENSORS_OUT_OF_BOUNDS_EVENT
};

void notifyAboutEvent(notification_event_t event);

#endif