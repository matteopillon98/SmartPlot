#ifndef HUMIDITYSENSORMANAGER_H
#define HUMIDITYSENSORMANAGER_H

class HumiditySensorManager {
public:
    HumiditySensorManager(int analogPin);
    void init();
    int readHumidity();

private:
    int pin;
};

#endif
