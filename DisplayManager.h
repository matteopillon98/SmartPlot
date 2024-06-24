#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <TFT_eSPI.h>

class DisplayManager {
public:
    DisplayManager();
    void init();
    void showMessage(const String &message);
    void showSensorData(int humidity);
    void showIcon(const uint8_t *bitmap, int16_t x, int16_t y, int16_t w, int16_t h);

private:
    TFT_eSPI display;
};

#endif
