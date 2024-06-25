#ifndef TEMPERATURESCREEN_H
#define TEMPERATURESCREEN_H

#include "Screen.h"

class TemperatureScreen : public Screen {
public:
    TemperatureScreen(float temperature);
    void display(TFT_eSPI &display) override;

private:
    float temperature;
};

#endif
