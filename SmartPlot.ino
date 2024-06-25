#include <WiFi.h>
#include <WiFiManager.h>
#include <TFT_eSPI.h>
#include "ScreenManager.h"
#include "SoilHumidityScreen.h"
#include "TemperatureScreen.h"
#include "HumiditySensorManager.h"

TFT_eSPI display = TFT_eSPI();  // Create display object
ScreenManager screenManager(display);  // Pass the display to the ScreenManager

HumiditySensorManager humiditySensor(34);  // Pin connected to the humidity sensor

void setup() {
    Serial.begin(115200);
    display.init();
    display.setRotation(2);  // Adjust as needed
    display.setTextSize(2);
    display.setFreeFont(&FreeSans9pt7b);  // Set the custom font


    humiditySensor.init();  // Initialize the humidity sensor

    // Initialize screens
    SoilHumidityScreen* soilHumidityScreen = new SoilHumidityScreen(&humiditySensor);
    TemperatureScreen* temperatureScreen = new TemperatureScreen(22.5);   // Example temperature value (commented as you don't have a sensor yet)

    screenManager.addScreen(soilHumidityScreen);
    screenManager.addScreen(temperatureScreen);  // Add temperature screen when sensor is available

    // Show the first screen
    screenManager.displayCurrentScreen();
}

void loop() {
    static unsigned long lastSwitchTime = 0;
    if (millis() - lastSwitchTime > 5000) {  // Switch screen every 5 seconds
        screenManager.nextScreen();
        screenManager.displayCurrentScreen();
        lastSwitchTime = millis();
    }
}
