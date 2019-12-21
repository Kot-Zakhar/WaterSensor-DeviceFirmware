#include <SensorChecker.h>

const int16_t radius = 100;
const int16_t minValue = 1000;
const int16_t maxValue = 3000;
int16_t lastSentValue = 0;
bool needToSendLetter = false;
bool notificationWasSent = false;

bool stateIsConfigForSensor;

Ticker sensorChecker;

bool SendNotificationAboutSensor();

void CheckSensorValue(){
    int16_t oldValue = GetSensorValue();
    int16_t newValue = UpdateSensorValue();

    if (newValue > oldValue + radius || newValue < oldValue - radius){
        log_v("Sensor - %d", newValue);
        
        if (newValue > maxValue || newValue < minValue){
            IOIndicate(SensorValueOutOfRange);
            needToSendLetter = true;           
        } else {
            IOIndicate(SensorValueCnaged);
            notificationWasSent = false;
        }
    }
}

void InitSensorChecker(bool isConfigState){
    InitSensorController();
    stateIsConfigForSensor = isConfigState;

    sensorChecker.attach(1, CheckSensorValue);
}

bool SendNotificationAboutSensor(){    
    if (stateIsConfigForSensor){
        IOWrite(IO_WRITE_SCREEN, "Sensor out of bounds!");
    } else {
        IOWrite(IO_WRITE_SCREEN, "Sending sensor letter");
        IOIndicate(WAIT);
        if (SendLetter("ESP32 Sensor", ("Sensor's value has gone out of bounds [" + String(minValue) + ": " + String(maxValue) + "].").c_str(), false, true)){
            IOIndicate(SUCCESS);
        } else {
            IOIndicate(ERROR);
            return false;
        }
    }
    return true;
}

void ProcessSensorChecker(){
    if (needToSendLetter && !stateIsConfigForSensor){
        if (!notificationWasSent){

            notificationWasSent = SendNotificationAboutSensor();
        } else {
            log_i("Sensor value still out of range - %d", GetSensorValue()); 
        }
        needToSendLetter = false;
    }
}