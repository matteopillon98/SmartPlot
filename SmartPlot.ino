#include <WiFi.h>
#include <TFT_eSPI.h>
#include "ScreenManager.h"
#include "SoilHumidityScreen.h"
#include "TemperatureScreen.h"
#include "HumiditySensorManager.h"
#include "BluetoothManager.h"

// BLE related
BluetoothManager bluetoothManager;
bool deviceConnected = false;
std::string receivedSsid = "";
std::string receivedPassword = "";

TFT_eSPI display = TFT_eSPI();  // Create display object
ScreenManager screenManager(display);  // Pass the display to the ScreenManager
HumiditySensorManager humiditySensor(34);  // Pin connected to the humidity sensor

void setup() {
  Serial.begin(115200);

  // Initialize Bluetooth
  bluetoothManager.init();
  bluetoothManager.setSsidCallback([](const std::string& ssid) {
    receivedSsid = ssid;
  });
  bluetoothManager.setPasswordCallback([](const std::string& password) {
    receivedPassword = password;
  });

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

  if (!receivedSsid.empty() && !receivedPassword.empty()) {
    Serial.println("Attempting to connect to WiFi with received credentials");
    
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
      attempts++;
    }

    if (wifiConnected) {
      Serial.println("Connected!");
      display.println("Connected!");
      bluetoothManager.notifyStatus("Connected to WiFi");
      // Delay before disconnecting BLE to allow the notification to be sent
      delay(1000);
      // Assume BLEClient is the client ID to be disconnected; this may need adjustment
      bluetoothManager.disconnectClient();  
      // Show regular screens
    } else {
      Serial.println("Failed to connect to WiFi");
      display.println("Failed to connect!");

      bluetoothManager.notifyStatus("Failed to connect to WiFi");
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
