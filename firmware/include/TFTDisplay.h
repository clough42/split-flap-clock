#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <Arduino.h>
#include <ILI9341_t3.h>
#include <XPT2046_Touchscreen.h>
#include "TimeData.h"

class TFTDisplay {
private:
    ILI9341_t3 tft_;
    XPT2046_Touchscreen touch_;
    TimeData lastDisplayedTime_;
    unsigned long lastUpdateTime_;
    
    void drawTimeDisplay();
    void drawBackground();
    bool timeChanged(const TimeData& newTime);
    
public:
    TFTDisplay(int tftCS, int tftDC, int tftRST, int touchCS, int touchIRQ);
    
    void initialize();
    void updateTime(const TimeData& timeData);
    void handleTouch();
    bool isTouchPressed();
};

#endif // TFT_DISPLAY_H