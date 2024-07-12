#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <TFT_eSPI.h>
#include "ScreenManager.h"
#include "SoilHumidityScreen.h"
#include "TemperatureScreen.h"
#include "HumiditySensorManager.h"

// BLE related
BLEServer* pServer = NULL;
BLECharacteristic* pSsidCharacteristic = NULL;
BLECharacteristic* pPasswordCharacteristic = NULL;
BLECharacteristic* pStatusCharacteristic = NULL;
bool deviceConnected = false;
std::string receivedSsid = "";
std::string receivedPassword = "";

// UUIDs for BLE service and characteristics
#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define SSID_CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-123456789abc"
#define PASSWORD_CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdefabcdef"
#define STATUS_CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdef123456"

TFT_eSPI display = TFT_eSPI();  // Create display object
ScreenManager screenManager(display);  // Pass the display to the ScreenManager
HumiditySensorManager humiditySensor(34);  // Pin connected to the humidity sensor

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Device connected");
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
  }
};

class SsidCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    receivedSsid = pCharacteristic->getValue().c_str();
    Serial.print("Received SSID: ");
    Serial.println(receivedSsid.c_str());
  }
};

class PasswordCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    receivedPassword = pCharacteristic->getValue().c_str();
    Serial.print("Received Password: ");
    Serial.println(receivedPassword.c_str());
  }
};

void setup() {
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("ESP32_BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics for SSID, Password, and Status
  pSsidCharacteristic = pService->createCharacteristic(
                         SSID_CHARACTERISTIC_UUID,
                         BLECharacteristic::PROPERTY_READ |
                         BLECharacteristic::PROPERTY_WRITE
                       );
  pSsidCharacteristic->setCallbacks(new SsidCharacteristicCallbacks());

  pPasswordCharacteristic = pService->createCharacteristic(
                             PASSWORD_CHARACTERISTIC_UUID,
                             BLECharacteristic::PROPERTY_READ |
                             BLECharacteristic::PROPERTY_WRITE
                           );
  pPasswordCharacteristic->setCallbacks(new PasswordCharacteristicCallbacks());

  pStatusCharacteristic = pService->createCharacteristic(
                            STATUS_CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_READ |
                            BLECharacteristic::PROPERTY_NOTIFY
                          );
  pStatusCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");

  // Initialize display
  display.init();
  display.setRotation(2);  // Adjust as needed
  display.setTextSize(2);
  display.setFreeFont(&FreeSans9pt7b);  // Set the custom font

  // Initialize humidity sensor
  humiditySensor.init();

  // Initialize screens
  SoilHumidityScreen* soilHumidityScreen = new SoilHumidityScreen(&humiditySensor);
  TemperatureScreen* temperatureScreen = new TemperatureScreen(22.5);   // Example temperature value
  screenManager.addScreen(soilHumidityScreen);
  screenManager.addScreen(temperatureScreen);

  // Initially show connection messages
  display.fillScreen(TFT_BLACK);
  display.setCursor(0, 0);
  display.println("Waiting for WiFi credentials...");
}

void loop() {
  static bool wifiConnected = false;
  static unsigned long lastSwitchTime = 0;

  if (deviceConnected && !receivedSsid.empty() && !receivedPassword.empty()) {
    Serial.println("Attempting to connect to WiFi with received credentials");

    // Show on display
    display.fillScreen(TFT_BLACK);
    display.setCursor(0, 0);
    display.println("Connecting to WiFi...");
    
    // Disconnect WiFi if already connected
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.disconnect();
      delay(1000);
    }

    WiFi.begin(receivedSsid.c_str(), receivedPassword.c_str());
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    wifiConnected = false;
    while (attempts < 20 && !wifiConnected) {
      if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
      }
      delay(500);
      Serial.print(".");
      display.print(".");
      attempts++;
    }

    if (wifiConnected) {
      Serial.println("Connected!");
      display.println("Connected!");

      pStatusCharacteristic->setValue("Connected to WiFi");
      pStatusCharacteristic->notify();
      
      // Delay before disconnecting BLE to allow the notification to be sent
      delay(1000);
      pServer->disconnect(0);  // Disconnect BLE client

      // Show regular screens
      screenManager.displayCurrentScreen();
    } else {
      Serial.println("Failed to connect to WiFi");
      display.println("Failed to connect!");

      pStatusCharacteristic->setValue("Failed to connect to WiFi");
      pStatusCharacteristic->notify();
    }

    // Clear received credentials after attempting to connect
    receivedSsid = "";
    receivedPassword = "";
  }

  if (wifiConnected) {
    if (millis() - lastSwitchTime > 5000) {  // Switch screen every 5 seconds
      screenManager.nextScreen();
      screenManager.displayCurrentScreen();
      lastSwitchTime = millis();
    }
  }
}
