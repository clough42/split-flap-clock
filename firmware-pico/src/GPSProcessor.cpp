#include <EEPROM.h>

#include "GPSProcessor.h"
#include "MechanicalDisplay.h"
#include "LEDController.h"
#include "TFTDisplay.h"
#include "Configuration.h"

GPSProcessor::GPSProcessor(ConfigPersistence& config, MechanicalDisplay& timeDisplay, TFTDisplay& displayController, LEDController& ledController, HardwareSerial& serial)
    : config_(config), timeDisplay_(timeDisplay), displayController_(displayController), ledController_(ledController), serial_(serial), is24HourFormat_(true) {
}

void GPSProcessor::setDisplayController(TFTDisplay& displayController) {
    displayController_ = displayController;
}

void GPSProcessor::incrementTimezoneOffset() {
    timezoneOffsetHours_++;
    if (timezoneOffsetHours_ > 23) {
        timezoneOffsetHours_ = 0;
    }
    config_.setTimezoneOffset(timezoneOffsetHours_);

    Serial.print("Timezone offset changed to: ");
    Serial.println(timezoneOffsetHours_);

    processGPSData(); // Reprocess GPS data to update display with new timezone
}

void GPSProcessor::initialize() {
    serial_.begin(GPS_BAUD_RATE);
    Serial.println("GPS serial interface initialized at 9600 baud");

    timezoneOffsetHours_ = config_.getTimezoneOffset();
    is24HourFormat_ = config_.getIs24HourFormat();
}
void GPSProcessor::toggleTimeFormat() {
    is24HourFormat_ = !is24HourFormat_;
    config_.setIs24HourFormat(is24HourFormat_);
    processGPSData(); // Update display
}

bool GPSProcessor::getIs24HourFormat() const {
    return is24HourFormat_;
}

void GPSProcessor::processIncomingData() {
    while (serial_.available()) {
        char gpsChar = serial_.read();
        
        // Feed character to TinyGPS++ for parsing
        if (gps_.encode(gpsChar)) {
            // TinyGPS++ has successfully parsed a complete sentence
            processGPSData();
        }
    }
}

const char* GPSProcessor::getSignalStrength(double hdop, int satellites) {
    // Not enough satellites = No Signal
    if (satellites < 3) {
        return "NO SIGNAL";
    }
    
    // Combine HDOP and satellite count for comprehensive assessment
    if (hdop < 1.0 && satellites >= 8) {
        return "EXCELLENT";
    } else if (hdop < 2.0 && satellites >= 6) {
        return "GOOD";
    } else if (hdop < 5.0 && satellites >= 4) {
        return "MODERATE";
    } else if (hdop < 10.0 && satellites >= 3) {
        return "FAIR";
    } else {
        return "POOR";
    }
}

void GPSProcessor::processGPSData() {
    // Always check for satellite data, even if time isn't valid yet
    int satelliteCount = gps_.satellites.value();
    bool timeIsValid = gps_.time.isValid() && satelliteCount >= 3;
    
    // Check if GPS status has changed since last update
    static int lastSatelliteCount = -1;
    static bool lastTimeValid = false;
    bool statusChanged = (satelliteCount != lastSatelliteCount) || (timeIsValid != lastTimeValid);
    
    // Always update display with current GPS status
    double hdop = gps_.hdop.hdop();
    const char* signalStrength = getSignalStrength(hdop, satelliteCount);
    TimeData timeData;


    if (timeIsValid) {
        // Extract valid time components
        int utcHours = gps_.time.hour();
        int utcMinutes = gps_.time.minute();
        int utcSeconds = gps_.time.second();

        // Calculate local time
        int localHours = utcHours + timezoneOffsetHours_;
        // Handle day rollover
        if (localHours < 0) {
            localHours += 24;
        } else if (localHours >= 24) {
            localHours -= 24;
        }

        int displayLocalHours = localHours;
        bool isPM = false;
        if (!is24HourFormat_) {
            // Convert to 12-hour format for display
            if (displayLocalHours == 0) {
                displayLocalHours = 12;
                isPM = false;
            } else if (displayLocalHours == 12) {
                isPM = true;
            } else if (displayLocalHours > 12) {
                displayLocalHours -= 12;
                isPM = true;
            }
        }

        // Populate time data with valid values
        timeData = {
            utcHours, utcMinutes, utcSeconds,
            displayLocalHours, utcMinutes, utcSeconds,
            satelliteCount,
            true,
            signalStrength,
            is24HourFormat_,
            isPM
        };

        // Toggle LED for valid GPS
        ledController_.toggle();

        // Update stepper motors
        timeDisplay_.updateTime(timeData);
    } else {
        // GPS data not valid yet, but show satellite status
        timeData = {0, 0, 0, 0, 0, 0, satelliteCount, false, signalStrength, is24HourFormat_, false};
    }

    // Always update display with current status
    displayController_.updateTime(timeData);
    
    // Only print status when something changes
    if (statusChanged) {
        double hdop = gps_.hdop.hdop();
        const char* signalStrength = getSignalStrength(hdop, satelliteCount);
        
        if (timeIsValid) {
            Serial.print("GPS Time Valid - Satellites: ");
            Serial.print(satelliteCount);
            Serial.print(" | HDOP: ");
            Serial.print(hdop, 1);
            Serial.print(" | Signal: ");
            Serial.println(signalStrength);
        } else {
            Serial.print("GPS acquiring... Satellites: ");
            Serial.print(satelliteCount);
            Serial.print(" | Signal: ");
            Serial.println(signalStrength);
        }
        
        // Update static variables to track changes
        lastSatelliteCount = satelliteCount;
        lastTimeValid = timeIsValid;
    }
}