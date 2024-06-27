Components Used
ESP32
TFT Display with GC9A01 driver
Soil Humidity Sensor
WiFi Module
(Optional) Temperature Sensor
Physical Connections
Here are the physical connections used in the project:

TFT Display

VCC to 3.3V
GND to GND
CS to pin 5
RESET to pin 22
DC to pin 21
MOSI to pin 23
SCK to pin 18
LED to 3.3V

Soil Humidity Sensor

VCC to 3.3V
GND to GND
Analog output to pin 34
WiFi Module

Integrated in ESP32
Software Setup
Install the necessary libraries:

TFT_eSPI
WiFi
WiFiManager
NTPClient
WiFiUdp
