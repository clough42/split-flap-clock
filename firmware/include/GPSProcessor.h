#ifndef GPS_PROCESSOR_H
#define GPS_PROCESSOR_H

#include <Arduino.h>
#include "TimeData.h"

// Forward declarations
class FlapDisplay;
class LEDController;
class TFTDisplay;

class GPSProcessor {
private:
    FlapDisplay* timeDisplay_;
    LEDController* ledController_;
    TFTDisplay* displayController_;
    HardwareSerial* serial_;
    String gpsBuffer_;
    int timezoneOffsetHours_;
    
    void parseGPSTime(const String& nmea);
    
public:
    GPSProcessor(int timezoneOffset, FlapDisplay* timeDisplay, LEDController* ledController, HardwareSerial* serial);
    
    void initialize();
    void processIncomingData();
    void setDisplayController(TFTDisplay* displayController);
};

#endif // GPS_PROCESSOR_H