#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include "TimeData.h"

class TFTDisplay {
public:
    TFTDisplay(int cs, int dc, int rst, int touchCs, int touchIrq) {}
    
    void updateTime(const TimeData& timeData) {
        Serial.print("TFT: ");
        if (timeData.validTime) {
            Serial.print("Valid GPS - Sats: ");
            Serial.print(timeData.satelliteCount);
            Serial.print(", Signal: ");
            Serial.println(timeData.signalStrength);
        } else {
            Serial.print("Acquiring GPS - Sats: ");
            Serial.print(timeData.satelliteCount);
            Serial.print(", Signal: ");
            Serial.println(timeData.signalStrength);
        }
    }
};

#endif // TFT_DISPLAY_H