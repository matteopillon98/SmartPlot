#include "HumiditySensorManager.h"
#include "DisplayManager.h"

#define HUMIDITY_SENSOR_PIN 34

DisplayManager displayManager;
HumiditySensorManager humiditySensorManager(HUMIDITY_SENSOR_PIN);

void setup() {
    Serial.begin(115200);

    displayManager.init();
    humiditySensorManager.init();

    int humidity = humiditySensorManager.readHumidity();
    displayManager.showSensorData(humidity);

    Serial.print("Soil Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
}

void loop() {
    int humidity = humiditySensorManager.readHumidity();
    displayManager.showSensorData(humidity);

    Serial.print("Soil Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    delay(2000);
}
