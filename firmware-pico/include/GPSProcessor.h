#ifndef GPS_PROCESSOR_H
#define GPS_PROCESSOR_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include "TimeData.h"

// Forward declarations
class MechanicalDisplay;
class LEDController;
class TFTDisplay;
#include "ConfigPersistence.h"

class GPSProcessor {
public:
    GPSProcessor(ConfigPersistence& config, MechanicalDisplay& timeDisplay, TFTDisplay& tftDisplay, LEDController& ledController, HardwareSerial& serial);

    void initialize();
    void processIncomingData();
    void setDisplayController(TFTDisplay& displayController);

    // Timezone offset management
    void incrementTimezoneOffset();

    // Time format management
    void toggleTimeFormat();
    bool getIs24HourFormat() const;

    // Signal strength assessment
    const char* getSignalStrength(double hdop, int satellites);

private:
    MechanicalDisplay& timeDisplay_;
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