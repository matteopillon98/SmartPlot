#include "SoilHumidityScreen.h"
#include "humidityIcon.h"  // Assicurati che il file dell'icona sia incluso

SoilHumidityScreen::SoilHumidityScreen(HumiditySensorManager* sensorManager) : sensorManager(sensorManager) {}

void SoilHumidityScreen::display(TFT_eSPI &display) {
    int humidity = sensorManager->readHumidity();  // Get the current humidity reading

    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_CYAN, TFT_BLACK);

    // Display the icon at the top center
    int iconX = (display.width() - 64) / 2;  // Adjust for your icon size
    int iconY = 20;  // Adjust for desired position
    display.drawBitmap(iconX, iconY, humidityIcon, 64, 64, TFT_CYAN);

    // Calculate the width of the first line text
    String firstLine = "Soil Humidity";
    int firstLineWidth = display.textWidth(firstLine.c_str());

    // Calculate the position to center the first line text
    int firstLineCenterX = (display.width() - firstLineWidth) / 2;

    // Display the first line centered
    display.setCursor(firstLineCenterX, display.height() / 2 + 20);
    display.println(firstLine);

    // Convert the humidity to a string and get its width
    String humidityStr = String(humidity) + "%";
    int percentageWidth = display.textWidth(humidityStr.c_str());

    // Calculate the position to center the percentage on the second line
    int percentageCenterX = (display.width() - percentageWidth) / 2;

    // Display the second line with the percentage centered
    display.setCursor(percentageCenterX, display.height() / 2 + 65);
    display.print(humidityStr);
}
