#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "TimeData.h"

class TFTDisplay {
private:
    Adafruit_ST7735 tft_;
    TimeData lastDisplayedTime_;
    unsigned long lastUpdateTime_;
    
    void drawTimeDisplay();
    void drawBackground();
    bool timeChanged(const TimeData& newTime);
    bool statusChanged(const TimeData& newTime);
    
public:
    TFTDisplay(int tftCS, int tftDC, int tftRST);
    
    void initialize();
    void updateTime(const TimeData& timeData);
};

#endif // TFT_DISPLAY_H