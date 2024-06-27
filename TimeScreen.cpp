#include "TimeScreen.h"
#include <time.h>

const char* dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char* monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

TimeScreen::TimeScreen(NTPClient &timeClient) : timeClient(timeClient) {}

bool TimeScreen::isDST(int day, int month, int weekday) {
    // DST in Italy starts on the last Sunday in March and ends on the last Sunday in October
    if (month < 3 || month > 10) return false; // Before last Sunday in March or after last Sunday in October
    if (month > 3 && month < 10) return true;  // Between April and September

    int previousSunday = day - weekday;

    // In March, DST starts if today is the last Sunday or later
    if (month == 3) return previousSunday >= 25;

    // In October, DST ends if today is the last Sunday or later
    if (month == 10) return previousSunday < 25;

    return false;  // Should never reach here
}

String TimeScreen::getFormattedTime() {
    time_t rawTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime(&rawTime);

    // Adjust for DST
    int dstOffset = isDST(ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_wday) ? 3600 : 0;
    rawTime += dstOffset;
    ptm = gmtime(&rawTime);

    char timeString[6];
    sprintf(timeString, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
    return String(timeString);
}

String TimeScreen::getFormattedDate() {
    time_t rawTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime(&rawTime);

    // Adjust for DST
    int dstOffset = isDST(ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_wday) ? 3600 : 0;
    rawTime += dstOffset;
    ptm = gmtime(&rawTime);

    char dateString[30];
    sprintf(dateString, "%s %d %s", dayNames[ptm->tm_wday], ptm->tm_mday, monthNames[ptm->tm_mon]);
    return String(dateString);
}

void TimeScreen::display(TFT_eSPI &display) {
    display.setTextColor(TFT_WHITE, TFT_BLACK); 
    display.setFreeFont(&FreeSans9pt7b);  // Set the custom font
    display.setTextSize(3);

    String timeStr = getFormattedTime();
    int16_t timeX = (display.width() - display.textWidth(timeStr.c_str())) / 2;
    display.setCursor(timeX, display.height() / 2);
    display.print(timeStr);

    display.setTextSize(1);
    String dateStr = getFormattedDate();
    int16_t dateX = (display.width() - display.textWidth(dateStr.c_str())) / 2;
    display.setCursor(dateX, display.height() / 2 + 30);  // Adjust the Y position as needed
    display.print(dateStr);
    display.setTextSize(2);

}
