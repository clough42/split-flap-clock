#include "TFTDisplay.h"
#include "Configuration.h"
#include "LEDController.h"

TFTDisplay::TFTDisplay(int tftCS, int tftDC, int tftRST)
    : tft_(tftCS, tftDC, tftRST) {
    // Initialize time data with safe defaults
    lastDisplayedTime_ = {0, 0, 0, 0, 0, 0, 0, false, "NO SIGNAL"};
    lastUpdateTime_ = 0;
}

void TFTDisplay::initialize() {
    // Adafruit ILI9341 uses hardware SPI (pins set in main before begin)
    tft_.begin();
    tft_.setRotation(1); // Landscape orientation (fixed upside-down)
    tft_.fillScreen(ILI9341_BLACK);

    // Draw initial background
    drawBackground();
}

void TFTDisplay::drawBackground() {
    tft_.fillScreen(ILI9341_BLACK);

    // Draw title
    tft_.setTextColor(ILI9341_WHITE);
    tft_.setTextSize(2);
    tft_.setCursor(50, 10);
    tft_.println("GPS Split-Flap Clock");

    // Draw divider line under title
    tft_.drawLine(0, 35, 320, 35, ILI9341_CYAN);

    // Draw time labels
    tft_.setTextSize(2);
    tft_.setCursor(20, 60);
    tft_.println("UTC Time:");
    tft_.setCursor(20, 120);
    tft_.println("Local Time:");
}

bool TFTDisplay::timeChanged(const TimeData& newTime) {
    return (newTime.utcHours != lastDisplayedTime_.utcHours ||
            newTime.utcMinutes != lastDisplayedTime_.utcMinutes ||
            newTime.utcSeconds != lastDisplayedTime_.utcSeconds ||
            newTime.localHours != lastDisplayedTime_.localHours ||
            newTime.localMinutes != lastDisplayedTime_.localMinutes ||
            newTime.localSeconds != lastDisplayedTime_.localSeconds);
}

bool TFTDisplay::statusChanged(const TimeData& newTime) {
    // Safe string comparison with null checks
    bool signalChanged = false;
    if (newTime.signalStrength != nullptr && lastDisplayedTime_.signalStrength != nullptr) {
        signalChanged = (strcmp(newTime.signalStrength, lastDisplayedTime_.signalStrength) != 0);
    } else if (newTime.signalStrength != lastDisplayedTime_.signalStrength) {
        signalChanged = true; // One is null, the other isn't
    }

    return (newTime.satelliteCount != lastDisplayedTime_.satelliteCount ||
            newTime.validTime != lastDisplayedTime_.validTime ||
            signalChanged);
}

void TFTDisplay::updateTime(const TimeData& timeData) {
    bool updated = false;

    // Only update if time has changed or it's been more than 1 second
    unsigned long currentTime = millis();
    if (timeChanged(timeData) || (currentTime - lastUpdateTime_) >= 1000) {
        lastUpdateTime_ = currentTime;

        // Display UTC time (draw directly over previous, no clearing needed)
        tft_.setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // Set background color
        tft_.setTextSize(3);
        tft_.setCursor(20, 85);
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d:%02d", timeData.utcHours, timeData.utcMinutes, timeData.utcSeconds);
        tft_.print(timeStr);

        // Display Local time (draw directly over previous, no clearing needed)
        tft_.setTextColor(ILI9341_GREEN, ILI9341_BLACK);  // Set background color
        tft_.setTextSize(3);
        tft_.setCursor(20, 145);
        sprintf(timeStr, "%02d:%02d:%02d", timeData.localHours, timeData.localMinutes, timeData.localSeconds);
        tft_.print(timeStr);

        updated = true;
    }

    if( statusChanged(timeData)) {
        // Color code based on signal strength
        uint16_t statusColor = ILI9341_WHITE;
        if (timeData.signalStrength) {
            if (strcmp(timeData.signalStrength, "EXCELLENT") == 0) statusColor = ILI9341_GREEN;
            else if (strcmp(timeData.signalStrength, "GOOD") == 0) statusColor = ILI9341_CYAN;
            else if (strcmp(timeData.signalStrength, "MODERATE") == 0) statusColor = ILI9341_YELLOW;
            else if (strcmp(timeData.signalStrength, "FAIR") == 0) statusColor = ILI9341_MAGENTA;
            else if (strcmp(timeData.signalStrength, "POOR") == 0) statusColor = ILI9341_RED;
            else if (strcmp(timeData.signalStrength, "NO SIGNAL") == 0) statusColor = ILI9341_RED;
        }

        char* lockStr = timeData.validTime ? (char *)"locked" : (char *)"unlocked";

        char statusStr[50];
        if (timeData.signalStrength) {
            sprintf(statusStr, "GPS: %s (%d satellites) %s", timeData.signalStrength, timeData.satelliteCount, lockStr);
        } else {
            sprintf(statusStr, "GPS: UNKNOWN (%d satellites) %s", timeData.satelliteCount, lockStr);
        }

        tft_.fillRect(20, 190, 280, 20, ILI9341_BLACK);
        tft_.setTextColor(statusColor);
        tft_.setTextSize(1);
        tft_.setCursor(20, 190);
        tft_.print(statusStr);

        updated = true;
    }

    if( updated ) {
        // Save last displayed time
        lastDisplayedTime_ = timeData;
    }
}
