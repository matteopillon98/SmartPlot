/*#include "WiFiManagerHandler.h"
#include <Arduino.h>

// Constructor
WiFiManagerHandler::WiFiManagerHandler(DisplayManager &displayManager) : display(displayManager) {}

// Function to connect to WiFi
void WiFiManagerHandler::connectWiFi() {
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(300);  // Timeout for configuration portal
    wifiManager.setSaveConfigCallback([this]() { this->saveConfigCallback(); });
    wifiManager.setAPCallback([this](WiFiManager *myWiFiManager) { this->configModeCallback(myWiFiManager); });
    wifiManager.setConfigPortalTimeoutCallback(configPortalTimeoutCallbackStatic);

    if (!wifiManager.autoConnect("AutoConnectAP")) {
        ESP.restart();
    }

    Serial.println("WiFi connected.");
    display.showMessage("WiFi Connection\nEstablished");
}

// Save configuration callback
void WiFiManagerHandler::saveConfigCallback() {
    Serial.println("WiFi configuration saved");
}

// Configuration mode callback
void WiFiManagerHandler::configModeCallback(WiFiManager *myWiFiManager) {
    Serial.println("Connect to " + myWiFiManager->getConfigPortalSSID());
    display.showMessage("Connect to\n" + myWiFiManager->getConfigPortalSSID() + "\nwith your\nsmartphone");
}

// Static configuration mode callback
void WiFiManagerHandler::configModeCallbackStatic(WiFiManager *myWiFiManager) {
    WiFiManagerHandler::configModeCallback(myWiFiManager);
}

// Configuration portal timeout callback
void WiFiManagerHandler::configPortalTimeoutCallback() {
    display.showMessage("Error during\nconfiguration");
    display.showMessage("Please restart\nyour device");
}

// Static configuration portal timeout callback
void WiFiManagerHandler::configPortalTimeoutCallbackStatic() {
    WiFiManagerHandler::configPortalTimeoutCallback();
}
*/