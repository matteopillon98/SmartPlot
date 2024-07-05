#include <WiFi.h>
#include <WiFiManager.h>
#include <TFT_eSPI.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ScreenManager.h"
#include "SoilHumidityScreen.h"
#include "TemperatureScreen.h"
#include "HumiditySensorManager.h"
#include "TimeScreen.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

TFT_eSPI display = TFT_eSPI();  // Create display object
ScreenManager screenManager(display);  // Pass the display to the ScreenManager

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

HumiditySensorManager humiditySensor(34);  // Pin connected to the humidity sensor
const char* ssid     = "TISCALI_D75E";
const char* password = "MGX48NRMQR";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
void setup() {
    Serial.begin(115200);
BLEDevice::init("ESP32_BLE");
    pServer = BLEDevice::createServer();
    
    // Crea un servizio BLE
    BLEService* pService = pServer->createService(BLEUUID((uint16_t)0xFFE0));

    // Crea una caratteristica BLE
    pCharacteristic = pService->createCharacteristic(
                        BLEUUID((uint16_t)0xFFE1),
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE
                      );

    pCharacteristic->setValue("Hello BLE");
    
    // Avvia il servizio
    pService->start();

    // Avvia la pubblicità BLE
    pServer->getAdvertising()->start();
    Serial.println("Waiting for a client connection to notify...");
    
        WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    display.init();
    display.setRotation(2);  // Adjust as needed
    display.setTextSize(2);
    display.setFreeFont(&FreeSans9pt7b);  // Set the custom font
    timeClient.begin();
    humiditySensor.init();  // Initialize the humidity sensor
    // Initialize screens
    SoilHumidityScreen* soilHumidityScreen = new SoilHumidityScreen(&humiditySensor);
    TemperatureScreen* temperatureScreen = new TemperatureScreen(22.5);   // Example temperature value (commented as you don't have a sensor yet)
    TimeScreen* timeScreen = new TimeScreen(timeClient);
    screenManager.addScreen(soilHumidityScreen);
    screenManager.addScreen(temperatureScreen);  // Add temperature screen when sensor is available
    screenManager.addScreen(timeScreen);  // Add the time screen
    // Show the first screen
    screenManager.displayCurrentScreen();
}

void loop() {
    static unsigned long lastSwitchTime = 0;
    timeClient.update();  // Update time from NTP server
    if (millis() - lastSwitchTime > 5000) {  // Switch screen every 5 seconds
      if (deviceConnected){
        pCharacteristic->setValue("Hello iPhone");
        pCharacteristic->notify();
      }
        screenManager.nextScreen();
        screenManager.displayCurrentScreen();
        lastSwitchTime = millis();
    }
}
