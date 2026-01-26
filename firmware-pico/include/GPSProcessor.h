#ifndef GPS_PROCESSOR_H
#define GPS_PROCESSOR_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include "TimeData.h"

// Forward declarations
class FlapDisplay;
class LEDController;
class TFTDisplay;

class GPSProcessor {
public:
    GPSProcessor(int timezoneOffset, FlapDisplay* timeDisplay, LEDController* ledController, HardwareSerial* serial);
    
    void initialize();
    void processIncomingData();
    void setDisplayController(TFTDisplay* displayController);
    
    // Timezone offset management
    void incrementTimezoneOffset();  // Increment timezone offset (0-23, wrapping)
    int getTimezoneOffset() const { return timezoneOffsetHours_; }
    
    // Signal strength assessment
    const char* getSignalStrength(double hdop, int satellites);

private:
    FlapDisplay* timeDisplay_;
    LEDController* ledController_;
    TFTDisplay* displayController_;
    HardwareSerial* serial_;
    TinyGPSPlus gps_;  // TinyGPS++ object
    int timezoneOffsetHours_;
    
    void processGPSData();  // Process parsed GPS data from TinyGPS++
};

#endif // GPS_PROCESSOR_H