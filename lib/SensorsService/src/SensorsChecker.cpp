#include "SensorsChecker.h"
#include "SensorsService.h"
#include <NotificationService.h>
#include <DHTController.h>
#include <WaterSensorController.h>
#include <MemoryController.h>
#include <custom_types.h>

#include <Ticker.h>

int waterSensorAccuracy = 100;
float tempAccuracy = 1.0;
float humidAccuracy = 1.0;

bool needToNotifyAboutSensors = false;
bool notificationWasSent = false;

void checkSensorsValue();
Ticker sensorsChecker(checkSensorsValue, 5000);

SensorsValues values;

int outOfBoundsSensors = 0;

void checkSensorsValue(){
    SensorsValues newValues;
    getCurrentSensorsValues(newValues);

    int waterLow, waterHigh;
    float tLow, tHigh, hLow, hHigh;
    bool water = getWaterSensorBoundariesFromMemory(waterLow, waterHigh);
    bool temp = getTemperatureBoundariesFromMemory(tLow, tHigh);
    bool humid = getHumidityBoundariesFromMemory(hLow, hHigh);

    if (newValues.water >= values.water + waterSensorAccuracy || newValues.water <= values.water - waterSensorAccuracy){
        log_d("Water sensor - %d", newValues.water);
        values.water = newValues.water;
        
        if (newValues.water >= waterHigh || newValues.water <= waterLow){
            outOfBoundsSensors = outOfBoundsSensors & WATER_SENSOR_TYPE;
        } else {
            outOfBoundsSensors = ~(~outOfBoundsSensors & WATER_SENSOR_TYPE);
        }
    }

    if (newValues.temperature >= values.temperature + tempAccuracy || newValues.temperature <= values.temperature - tempAccuracy){
        log_d("Temperature - %.0f*C", newValues.temperature);
        values.temperature = newValues.temperature;
        
        if (newValues.temperature >= tHigh || newValues.temperature <= tLow){
            outOfBoundsSensors = outOfBoundsSensors & TEMPERATURE_SENSOR_TYPE;
        } else {
            outOfBoundsSensors = ~(~outOfBoundsSensors & TEMPERATURE_SENSOR_TYPE);
        }
    }

    if (newValues.humidity >= values.humidity + humidAccuracy || newValues.humidity <= values.humidity - humidAccuracy){
        log_d("Humidity - %.0f%%", newValues.humidity);
        values.humidity = newValues.humidity;
        
        if (newValues.humidity >= hHigh || newValues.humidity <= hLow){
            outOfBoundsSensors = outOfBoundsSensors & HUMIDITY_SENSOR_TYPE;
        } else {
            outOfBoundsSensors = ~(~outOfBoundsSensors & HUMIDITY_SENSOR_TYPE);
        }
    }

    if (outOfBoundsSensors) {
        needToNotifyAboutSensors = true;
    } else {
        if (!needToNotifyAboutSensors)
            // resetting to initial state
            notificationWasSent = false;
    }
}

void initSensorsChecker() {
    initWaterSensorController();
    initDHTController();
    getCurrentSensorsValues(values);
    sensorsChecker.start();
}

bool sensorsNeedToBeProcessed() {
    sensorsChecker.update();
    return needToNotifyAboutSensors;
}

void processSensorsChecker(){
    if (needToNotifyAboutSensors){
        if (!notificationWasSent){
            // TODO: check actual result of notifyAboutEvent to resend notification on failure
            notifyAboutEvent(SENSORS_OUT_OF_BOUNDS_EVENT);
            notificationWasSent = true;
        } else {
            log_i("Sensor value still out of range - %d", getWaterSensorValue()); 
        }
        needToNotifyAboutSensors = false;
    }
}