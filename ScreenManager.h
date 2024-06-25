#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <vector>
#include "Screen.h"

class ScreenManager {
public:
    ScreenManager(TFT_eSPI& display);
    void addScreen(Screen* screen);
    void nextScreen();
    void displayCurrentScreen();

private:
    TFT_eSPI& display;
    std::vector<Screen*> screens;
    int currentScreenIndex;
};

#endif
