#include "TimeScreen.h"
#include <TFT_eSPI.h>

// Define dayNames and monthNames arrays
const char* dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char* monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

TimeScreen::TimeScreen(NTPClient &timeClient) : timeClient(timeClient) {
}

void TimeScreen::display(TFT_eSPI &display) {
    String currentTime = getFormattedTime();
    String currentDate = getFormattedDate();

    display.fillScreen(TFT_BLACK);
    display.setTextColor(TFT_CYAN, TFT_BLACK);
    display.setFreeFont(&FreeSans9pt7b);  // Set the custom font
    display.setTextSize(3);

    // Get the formatted time and display it centered
    int16_t timeX = (display.width() - display.textWidth(currentTime.c_str())) / 2;
    display.setCursor(timeX, display.height() / 2 );
    display.println(currentTime);

    display.setTextSize(2);

    timeX = (display.width() - display.textWidth(currentDate.c_str())) / 2;
    display.setCursor(timeX, display.height() / 2 + 65);
    display.println(currentDate);

}

String TimeScreen::getFormattedTime() {
    char orario [20];
    time_t now = timeClient.getEpochTime();
    struct tm timeInfo = *localtime(&now);
    //strftime(orario, sizeof(orario), "%d/%m/%Y -- %H:%M", &timeInfo );
    strftime(orario, sizeof(orario), "%H:%M", &timeInfo );

    return String(orario);
}

String TimeScreen::getFormattedDate() {
    char data [50];
    time_t now = timeClient.getEpochTime();
    struct tm timeInfo = *localtime(&now);
    strftime(data, sizeof(data), "%d/%m/%Y", &timeInfo );
    return String(data);
}