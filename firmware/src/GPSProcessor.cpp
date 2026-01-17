#include "GPSProcessor.h"
#include "TimeDisplay.h"
#include "LEDController.h"

GPSProcessor::GPSProcessor(int timezoneOffset, TimeDisplay* timeDisplay, LEDController* ledController, HardwareSerial* serial) 
    : timeDisplay_(timeDisplay), ledController_(ledController), serial_(serial),
      gpsBuffer_(""), timezoneOffsetHours_(timezoneOffset) {
}

void GPSProcessor::initialize() {
    serial_->begin(9600);
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
    // Look for GPGGA sentences which contain time data
    if (nmea.startsWith("$GPGGA") || nmea.startsWith("$GNGGA")) {
        // Split the sentence by commas
        int firstComma = nmea.indexOf(',');
        int secondComma = nmea.indexOf(',', firstComma + 1);
        
        if (firstComma != -1 && secondComma != -1) {
            // Extract time field (between first and second comma)
            String timeStr = nmea.substring(firstComma + 1, secondComma);
            
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
            }
        }
    }
}