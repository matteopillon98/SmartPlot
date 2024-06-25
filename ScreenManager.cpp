#include "ScreenManager.h"

ScreenManager::ScreenManager(TFT_eSPI& display) : display(display), currentScreenIndex(0) {}

void ScreenManager::addScreen(Screen* screen) {
    screens.push_back(screen);
}

void ScreenManager::nextScreen() {
    currentScreenIndex = (currentScreenIndex + 1) % screens.size();
}

void ScreenManager::displayCurrentScreen() {
    if (!screens.empty()) {
        display.fillScreen(TFT_BLACK);
        screens[currentScreenIndex]->display(display);
    }
}
