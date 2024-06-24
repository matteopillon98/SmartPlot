/*#ifndef WIFIMANAGERHANDLER_H
#define WIFIMANAGERHANDLER_H

#include "DisplayManager.h"
#include <WiFiManager.h>

class WiFiManagerHandler {
public:
    WiFiManagerHandler(DisplayManager &displayManager);
    void connectWiFi();

private:
    DisplayManager &display;
    void saveConfigCallback();
    void configModeCallback(WiFiManager *myWiFiManager);
    static void configModeCallbackStatic(WiFiManager *myWiFiManager);
    void configPortalTimeoutCallback();
    static void configPortalTimeoutCallbackStatic();
};

#endif
*/