#include "TemperatureScreen.h"
#include "roomTemperatureIcon.h"

TemperatureScreen::TemperatureScreen(float temperature) : temperature(temperature) {}

void TemperatureScreen::display(TFT_eSPI &display) {
    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_RED, TFT_BLACK);
    display.setFreeFont(&FreeSans9pt7b);  // Set the custom font

     //Display the icon at the top center (if you have one for temperature)
     int iconX = (display.width() - 64) / 2;  // Adjust for your icon size
     int iconY = 20;  // Adjust for desired position
     display.drawBitmap(iconX, iconY, roomTemperatureIcon, 64, 64, TFT_RED);

    // Calculate the width of the first line text
    String firstLine = "Temperature";
    int firstLineWidth = display.textWidth(firstLine.c_str());

    // Calculate the position to center the first line text
    int firstLineCenterX = (display.width() - firstLineWidth) / 2;

    // Display the first line centered
    display.setCursor(firstLineCenterX, display.height() / 2 + 20);
    display.println(firstLine);

    // Convert the temperature to a string and get its width
    String temperatureStr = String(temperature, 1) + "°C";
    int temperatureWidth = display.textWidth(temperatureStr.c_str());

    // Calculate the position to center the temperature on the second line
    int temperatureCenterX = (display.width() - temperatureWidth) / 2;

    // Display the second line with the temperature centered
    display.setCursor(temperatureCenterX, display.height() / 2 + 60);
    display.print(temperatureStr);

}
