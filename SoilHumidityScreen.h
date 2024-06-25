#ifndef SOILHUMIDITYSCREEN_H
#define SOILHUMIDITYSCREEN_H

#include "Screen.h"
#include "HumiditySensorManager.h"

class SoilHumidityScreen : public Screen {
public:
    SoilHumidityScreen(HumiditySensorManager* sensorManager);
    void display(TFT_eSPI &display) override;

private:
    HumiditySensorManager* sensorManager;
};

#endif
