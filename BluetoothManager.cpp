#include "BluetoothManager.h"
#include <Arduino.h>

// UUIDs for BLE service and characteristics
#define SERVICE_UUID "12345678-1234-1234-1234-123456789abc"
#define SSID_CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-123456789abc"
#define PASSWORD_CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdefabcdef"
#define STATUS_CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdef123456"

BluetoothManager::BluetoothManager() : 
  pServer(nullptr), pSsidCharacteristic(nullptr), 
  pPasswordCharacteristic(nullptr), pStatusCharacteristic(nullptr),
  ssidCallback(nullptr), passwordCallback(nullptr) {}

void BluetoothManager::init() {
  // Initialize BLE
  BLEDevice::init("ESP32_BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks(this));

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics for SSID, Password, and Status
  pSsidCharacteristic = pService->createCharacteristic(
                         SSID_CHARACTERISTIC_UUID,
                         BLECharacteristic::PROPERTY_READ |
                         BLECharacteristic::PROPERTY_WRITE
                       );
  pSsidCharacteristic->setCallbacks(new SsidCharacteristicCallbacks(this));

  pPasswordCharacteristic = pService->createCharacteristic(
                             PASSWORD_CHARACTERISTIC_UUID,
                             BLECharacteristic::PROPERTY_READ |
                             BLECharacteristic::PROPERTY_WRITE
                           );
  pPasswordCharacteristic->setCallbacks(new PasswordCharacteristicCallbacks(this));

  pStatusCharacteristic = pService->createCharacteristic(
                            STATUS_CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_READ |
                            BLECharacteristic::PROPERTY_NOTIFY
                          );
  pStatusCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify...");
}

void BluetoothManager::setSsidCallback(void (*callback)(const std::string&)) {
  ssidCallback = callback;
}

void BluetoothManager::setPasswordCallback(void (*callback)(const std::string&)) {
  passwordCallback = callback;
}

void BluetoothManager::notifyStatus(const std::string& status) {
  if (pStatusCharacteristic) {
    pStatusCharacteristic->setValue(String(status.c_str())); // Convert std::string to String
    pStatusCharacteristic->notify();
  }
}

void BluetoothManager::disconnectClient() {
  if (pServer) {
    pServer->disconnect(0);
  }
}

// MyServerCallbacks implementation
void MyServerCallbacks::onConnect(BLEServer* pServer) {
  Serial.println("Device connected");
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
  Serial.println("Device disconnected");
}

// SsidCharacteristicCallbacks implementation
void SsidCharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
  manager->receivedSsid = pCharacteristic->getValue().c_str();
  Serial.print("Received SSID: ");
  Serial.println(manager->receivedSsid.c_str());
  if (manager->ssidCallback) {
    manager->ssidCallback(manager->receivedSsid);
  }
}

// PasswordCharacteristicCallbacks implementation
void PasswordCharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
  manager->receivedPassword = pCharacteristic->getValue().c_str();
  Serial.print("Received Password: ");
  Serial.println(manager->receivedPassword.c_str());
  if (manager->passwordCallback) {
    manager->passwordCallback(manager->receivedPassword);
  }
}
