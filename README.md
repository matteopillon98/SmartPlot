# Components Used
- ESP32
- TFT Display with GC9A01 driver
- Soil Humidity Sensor
- WiFi Module
- Temperature Sensor
- Physical Connections
- Here are the physical connections used in the project:

## TFT Display Configuration

- VCC to 3.3V
- GND to GND
- SDA to pin 23
- SCK to pin 5
- CS to pin 15
- DC to pin 2
- LED to 3.3V
- RESET to pin 4

## Soil Humidity Sensor Configuration

- VCC to 3.3V
- GND to GND
- Analog output to pin 34

## WiFi Module

- Integrated in ESP32

# Install the necessary libraries:

- TFT_eSPI
- WiFi
- WiFiManager
- NTPClient
- WiFiUdp
