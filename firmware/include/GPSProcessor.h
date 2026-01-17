#ifndef GPS_PROCESSOR_H
#define GPS_PROCESSOR_H

#include <Arduino.h>

// Forward declarations
class TimeDisplay;
class LEDController;

class GPSProcessor {
private:
    TimeDisplay* timeDisplay_;
    LEDController* ledController_;
    HardwareSerial* serial_;
    String gpsBuffer_;
    int timezoneOffsetHours_;
    
    void parseGPSTime(const String& nmea);
    
public:
    GPSProcessor(int timezoneOffset, TimeDisplay* timeDisplay, LEDController* ledController, HardwareSerial* serial);
    
    void initialize();
    void processIncomingData();
};

#endif // GPS_PROCESSOR_H