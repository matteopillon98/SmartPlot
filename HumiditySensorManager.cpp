#include "HumiditySensorManager.h"
#include <Arduino.h>

HumiditySensorManager::HumiditySensorManager(int analogPin) : pin(analogPin) {}

void HumiditySensorManager::init() {
    pinMode(pin, INPUT);
}

int HumiditySensorManager::readHumidity() {
    int analogValue = analogRead(pin);
    int percentageHumidity = map(analogValue, 0, 4095, 100, 0);  // Convert analog value to percentage
    return percentageHumidity;
}
