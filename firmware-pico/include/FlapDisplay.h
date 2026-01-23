#ifndef FLAP_DISPLAY_H
#define FLAP_DISPLAY_H

#include "TimeData.h"

class FlapDisplay {
public:
    FlapDisplay() {}
    
    void updateTime(const TimeData& timeData) {
        if (timeData.validTime) {
            Serial.print("FLAP: Valid time - ");
            Serial.print(timeData.localHours);
            Serial.print(":");
            if (timeData.localMinutes < 10) Serial.print("0");
            Serial.print(timeData.localMinutes);
            Serial.print(":");
            if (timeData.localSeconds < 10) Serial.print("0");
            Serial.print(timeData.localSeconds);
            Serial.print(" (UTC ");
            Serial.print(timeData.utcHours);
            Serial.print(":");
            if (timeData.utcMinutes < 10) Serial.print("0");
            Serial.print(timeData.utcMinutes);
            Serial.print(":");
            if (timeData.utcSeconds < 10) Serial.print("0");
            Serial.print(timeData.utcSeconds);
            Serial.println(")");
        }
    }
};

#endif // FLAP_DISPLAY_H