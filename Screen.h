#ifndef SCREEN_H
#define SCREEN_H

#include <TFT_eSPI.h>  // Include TFT_eSPI here

class Screen {
public:
    virtual void display(TFT_eSPI &display) = 0;
};

#endif
