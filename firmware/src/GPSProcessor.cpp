#include "GPSProcessor.h"
#include "FlapDisplay.h"
#include "LEDController.h"
#include "TFTDisplay.h"
#include "Configuration.h"

GPSProcessor::GPSProcessor(int timezoneOffset, FlapDisplay* timeDisplay, LEDController* ledController, HardwareSerial* serial) 
    : timeDisplay_(timeDisplay), ledController_(ledController), displayController_(nullptr), serial_(serial),
      gpsBuffer_(""), timezoneOffsetHours_(timezoneOffset) {
}

void GPSProcessor::setDisplayController(TFTDisplay* displayController) {
    displayController_ = displayController;
}

void GPSProcessor::initialize() {
    serial_->begin(GPS_BAUD_RATE);
    Serial.println("GPS serial interface initialized at 9600 baud");
}

void GPSProcessor::processIncomingData() {
    while (serial_->available()) {
        char gpsChar = serial_->read();
        
        // Add character to buffer
        gpsBuffer_ += gpsChar;
        
        // Check if we have a complete line (NMEA sentences end with \r\n)
        if (gpsChar == '\n') {
            // Parse and display GPS time
            parseGPSTime(gpsBuffer_);
            
            // Clear buffer for next line
            gpsBuffer_ = "";
        }
    }
}

void GPSProcessor::parseGPSTime(const String& nmea) {
    // Look for GPGGA sentences which contain time data and satellite count
    if (nmea.startsWith("$GPGGA") || nmea.startsWith("$GNGGA")) {
        // GPGGA format: $GPGGA,time,lat,latNS,lon,lonEW,quality,numSV,HDOP,alt,altUnits,sep,sepUnits,diffAge,diffStation*checksum
        // We want: time (field 1) and numSV (field 7 - number of satellites)
        
        // Split the sentence by commas
        int commaPositions[15];  // Store positions of first 15 commas
        int commaCount = 0;
        
        for (unsigned int i = 0; i < nmea.length() && commaCount < 15; i++) {
            if (nmea.charAt(i) == ',') {
                commaPositions[commaCount] = i;
                commaCount++;
            }
        }
        
        if (commaCount >= 7) {
            // Extract time field (field 1)
            String timeStr = nmea.substring(commaPositions[0] + 1, commaPositions[1]);
            
            // Extract satellite count (field 7)
            String satCountStr = nmea.substring(commaPositions[6] + 1, commaPositions[7]);
            int satelliteCount = satCountStr.toInt();
            
            if (timeStr.length() >= 6) {
                // Parse HHMMSS.SS format
                int utcHours = timeStr.substring(0, 2).toInt();
                int utcMinutes = timeStr.substring(2, 4).toInt();
                int utcSeconds = timeStr.substring(4, 6).toInt();
                
                // Calculate local time
                int localHours = utcHours + timezoneOffsetHours_;
                
                // Handle day rollover
                if (localHours < 0) {
                    localHours += 24;
                } else if (localHours >= 24) {
                    localHours -= 24;
                }
                
                // Display UTC time
                Serial.print("GPS Time (UTC): ");
                if (utcHours < 10) Serial.print("0");
                Serial.print(utcHours);
                Serial.print(":");
                if (utcMinutes < 10) Serial.print("0");
                Serial.print(utcMinutes);
                Serial.print(":");
                if (utcSeconds < 10) Serial.print("0");
                Serial.print(utcSeconds);
                
                // Add fractional seconds if available
                if (timeStr.length() > 6) {
                    Serial.print(timeStr.substring(6));
                }
                
                // Display local time
                Serial.print(" | Local Time: ");
                if (localHours < 10) Serial.print("0");
                Serial.print(localHours);
                Serial.print(":");
                if (utcMinutes < 10) Serial.print("0");
                Serial.print(utcMinutes);
                Serial.print(":");
                if (utcSeconds < 10) Serial.print("0");
                Serial.print(utcSeconds);
                
                // Add fractional seconds if available
                if (timeStr.length() > 6) {
                    Serial.print(timeStr.substring(6));
                }
                Serial.println();
                
                // Toggle LED to indicate successful GPS time parsing
                if (ledController_) {
                    ledController_->toggle();
                }
                
                // Update stepper motors to display time
                if (timeDisplay_) {
                    timeDisplay_->updateTime(localHours, utcMinutes);
                }
                
                // Update TFT display with time data
                if (displayController_) {
                    TimeData timeData = {
                        utcHours, utcMinutes, utcSeconds,
                        localHours, utcMinutes, utcSeconds,  // Note: using utcMinutes for local minutes too
                        satelliteCount,
                        true
                    };
                    displayController_->updateTime(timeData);
                }
            }
        }
    }
}