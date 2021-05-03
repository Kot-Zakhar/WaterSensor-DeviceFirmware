#include "DHTController.h"

#define DHT_PIN GPIO_NUM_15
#define DHT_TYPE DHT11

DHT_Unified dht(DHT_PIN, DHT_TYPE);
uint32_t delayMS;

void initDHTController() {
  dht.begin();
}

int getTemperature(float *value) {
    sensors_event_t event; 
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        return 1;
    }
    *value = event.temperature;
    return 0;
}

int getHumidity(float *value) {
    sensors_event_t event; 
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        return 1;
    }
    *value = event.relative_humidity;
    return 0;
}