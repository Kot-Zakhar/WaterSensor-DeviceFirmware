#include <SensorChecker.h>

const int16_t radius = 100;
const int16_t minValue = 1000;
const int16_t maxValue = 3000;
int16_t lastSentValue = 0;
bool needToSendLetter = false;
bool notificationWasSent = false;

bool stateIsConfigForSensor;

Ticker sensorChecker;

bool sendNotificationAboutSensor();

void checkSensorValue(){
    int16_t oldValue = getSensorValue();
    int16_t newValue = updateSensorValue();

    if (newValue > oldValue + radius || newValue < oldValue - radius){
        log_v("Sensor - %d", newValue);
        
        if (newValue > maxValue || newValue < minValue){
            ioIndicate(SensorValueOutOfRange);
            needToSendLetter = true;           
        } else {
            ioIndicate(SensorValueCnaged);
            notificationWasSent = false;
        }
    }
}

void initSensorChecker(bool isConfigState){
    initSensorController();
    initDHTController();
    stateIsConfigForSensor = isConfigState;

    sensorChecker.attach(1, checkSensorValue);
}

void stopSensorChecker() {
    sensorChecker.detach();
}

void restartSensorChecker() {
    sensorChecker.attach(1, checkSensorValue);
}

bool sendNotificationAboutSensor(){    
    if (stateIsConfigForSensor){
        ioWrite(IO_WRITE_SCREEN, "Sensor out of bounds!");
    } else {
        ioWrite(IO_WRITE_SCREEN, "Sending sensor letter");
        ioIndicate(WAIT);
        if (sendLetter("ESP32 Sensor", ("Sensor's value has gone out of bounds [" + String(minValue) + ": " + String(maxValue) + "].").c_str(), false, true)){
            ioIndicate(SUCCESS);
        } else {
            ioIndicate(ERROR);
            return false;
        }
    }
    return true;
}

bool shouldSensorBeProcessed() {
    return needToSendLetter && !stateIsConfigForSensor;
}

void processSensorChecker(){
    if (needToSendLetter && !stateIsConfigForSensor){
        if (!notificationWasSent){

            notificationWasSent = sendNotificationAboutSensor();
        } else {
            log_i("Sensor value still out of range - %d", getSensorValue()); 
        }
        needToSendLetter = false;
    }
}