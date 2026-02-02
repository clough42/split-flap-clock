#ifndef GPS_PROCESSOR_H
#define GPS_PROCESSOR_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include "TimeData.h"

// Forward declarations
class FlapDisplay;
class LEDController;
class TFTDisplay;
#include "ConfigPersistence.h"

class GPSProcessor {
public:
    GPSProcessor(ConfigPersistence& config, FlapDisplay& timeDisplay, TFTDisplay& tftDisplay, LEDController& ledController, HardwareSerial& serial);

    void initialize();
    void processIncomingData();
    void setDisplayController(TFTDisplay& displayController);

    // Timezone offset management
    void incrementTimezoneOffset();  // Increment timezone offset (0-23, wrapping)

    // Time format management
    void toggleTimeFormat();
    bool getIs24HourFormat() const;

    // Signal strength assessment
    const char* getSignalStrength(double hdop, int satellites);

private:
    FlapDisplay& timeDisplay_;
    LEDController& ledController_;
    TFTDisplay& displayController_;
    HardwareSerial& serial_;
    TinyGPSPlus gps_;  // TinyGPS++ object
    ConfigPersistence& config_;
    int timezoneOffsetHours_;
    bool is24HourFormat_;

    void processGPSData();  // Process parsed GPS data from TinyGPS++
};

#endif // GPS_PROCESSOR_H