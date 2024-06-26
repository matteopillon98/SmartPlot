#ifndef TIME_SCREEN_H
#define TIME_SCREEN_H

#include "Screen.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

class TimeScreen : public Screen {
public:
    TimeScreen(NTPClient &timeClient);
    void display(TFT_eSPI &display) override;

private:
    NTPClient &timeClient;
    String getFormattedTime();
    String getFormattedDate();

};

#endif
