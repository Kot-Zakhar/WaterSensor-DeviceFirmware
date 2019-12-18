#include <SensorController.h>

#define SENSOR_PIN 39

const int16_t radius = 100;
const int16_t minValue = 1000;
const int16_t maxValue = 3000;
int16_t value = 0;
int16_t lastSentValue = 0;
bool notificationWasSent = false;
bool stateIsConfigForSensor;

Ticker checker;

bool SendNotificationAboutSensor();


void UpdateSensorValue(){
    int16_t newValue = analogRead(SENSOR_PIN);
    int16_t oldValue = value;

    if (newValue > oldValue + radius || newValue < oldValue - radius){
        log_v("Sensor - %d", newValue);
        value = newValue;
        
        if (newValue > maxValue || newValue < minValue){
            IOIndicate(SensorValueOutOfRange);
            if (!notificationWasSent){
                notificationWasSent = SendNotificationAboutSensor();
            } else
                if ((newValue > maxValue && newValue > oldValue) || (newValue < minValue && newValue < oldValue))
                    log_i("Sensor value became worse - %d", newValue);            
        } else {
            IOIndicate(SensorValueCnaged);
        }
    }
}

bool SendNotificationAboutSensor(){
    if (notificationWasSent)
        return true;
    
    if (stateIsConfigForSensor){
        IOWrite(IO_WRITE_SCREEN, "Sensor out of bounds!");
    } else {
        IOWrite(IO_WRITE_SCREEN, "Sending sensor letter.");
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

void InitSensorController(bool isConfigState){
    pinMode(SENSOR_PIN, INPUT);
    value = analogRead(SENSOR_PIN);

    stateIsConfigForSensor = isConfigState;

    checker.attach(1, UpdateSensorValue);
}

int GetSensorValue(){
    return value;
}
