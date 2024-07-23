#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Forward declaration of the callback classes
class MyServerCallbacks;
class SsidCharacteristicCallbacks;
class PasswordCharacteristicCallbacks;

class BluetoothManager {
public:
  BluetoothManager();
  void init();
  void setSsidCallback(void (*callback)(const std::string&));
  void setPasswordCallback(void (*callback)(const std::string&));
  void notifyStatus(const std::string& status);
  void disconnectClient();

private:
  BLEServer* pServer;
  BLECharacteristic* pSsidCharacteristic;
  BLECharacteristic* pPasswordCharacteristic;
  BLECharacteristic* pStatusCharacteristic;
  std::string receivedSsid;
  std::string receivedPassword;

  friend class MyServerCallbacks;
  friend class SsidCharacteristicCallbacks;
  friend class PasswordCharacteristicCallbacks;
  
  void (*ssidCallback)(const std::string&);
  void (*passwordCallback)(const std::string&);
};

class MyServerCallbacks : public BLEServerCallbacks {
public:
  MyServerCallbacks(BluetoothManager* manager) : manager(manager) {}
  void onConnect(BLEServer* pServer);
  void onDisconnect(BLEServer* pServer);
private:
  BluetoothManager* manager;
};

class SsidCharacteristicCallbacks : public BLECharacteristicCallbacks {
public:
  SsidCharacteristicCallbacks(BluetoothManager* manager) : manager(manager) {}
  void onWrite(BLECharacteristic* pCharacteristic);
private:
  BluetoothManager* manager;
};

class PasswordCharacteristicCallbacks : public BLECharacteristicCallbacks {
public:
  PasswordCharacteristicCallbacks(BluetoothManager* manager) : manager(manager) {}
  void onWrite(BLECharacteristic* pCharacteristic);
private:
  BluetoothManager* manager;
};

#endif // BLUETOOTH_MANAGER_H
