#include "DisplayManager.h"
#include <Arduino.h>
#include "humidityIcon.h"  // Include the icon header file


DisplayManager::DisplayManager() : display(TFT_eSPI()) {}

void DisplayManager::init() {
    display.init();
    display.setRotation(2);
    display.setTextSize(2);
    display.fillScreen(TFT_BLACK);
}

void DisplayManager::showMessage(const String &message) {
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setCursor(10, display.height() / 2 - 30);
    display.println(message);
    delay(4000); 
}

void DisplayManager::showSensorData(int humidity) {
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_CYAN, TFT_BLACK);
    display.setFreeFont(&FreeSans9pt7b);  // Set the custom font

    // Display the icon at the top center
    showIcon(humidityIcon, (display.width() - 64) / 2, 20, 64, 64);

    // Calculate the width of the first line text
    String firstLine = "Soil Humidity";
    int firstLineWidth = display.textWidth(firstLine.c_str());

    // Calculate the position to center the first line text
    int firstLineCenterX = (display.width() - firstLineWidth) / 2;

    // Display the first line centered
    display.setCursor(firstLineCenterX, display.height() / 2 + 10);
    display.println(firstLine);

    // Convert the humidity to a string and get its width
    String humidityStr = String(humidity) + "%";
    int percentageWidth = display.textWidth(humidityStr.c_str());

    // Calculate the position to center the percentage on the second line
    int percentageCenterX = (display.width() - percentageWidth) / 2;

    // Display the second line with the percentage centered
    display.setCursor(percentageCenterX, display.height() / 2 + 60);
    display.print(humidityStr);

    delay(4000);
}

void DisplayManager::showIcon(const uint8_t *bitmap, int16_t x, int16_t y, int16_t w, int16_t h) {
    display.drawBitmap(x, y, bitmap, w, h, TFT_CYAN);
}