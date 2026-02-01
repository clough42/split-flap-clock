
#include "TFTDisplay.h"
#include "Configuration.h"
#include "LEDController.h"

// For ST7735, use Adafruit_ST7735(tftCS, tftDC, tftRST)
TFTDisplay::TFTDisplay(int tftCS, int tftDC, int tftRST)
    : tft_(tftCS, tftDC, tftRST) {
    lastDisplayedTime_ = {0, 0, 0, 0, 0, 0, 0, false, "NO SIGNAL"};
    lastUpdateTime_ = 0;
}

void TFTDisplay::initialize() {
    // Adafruit ST7735 uses hardware SPI (pins set in main before begin)
    tft_.initR(INITR_MINI160x80); // 80x160 ST7735
    tft_.setRotation(1); // Landscape orientation
    tft_.fillScreen(ST77XX_BLACK);
    drawBackground();
}

void TFTDisplay::drawBackground() {
    tft_.fillScreen(ST77XX_BLACK);
    // No title, no divider, no labels here; labels will be drawn with the time in updateTime
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

        // Display UTC time
        tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft_.setTextSize(2);
        tft_.setCursor(2, 6);
        tft_.print("UTC:");
        tft_.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
        tft_.setCursor(60, 6);
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d:%02d", timeData.utcHours, timeData.utcMinutes, timeData.utcSeconds);
        tft_.print(timeStr);

        // Display Local time
        tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft_.setTextSize(2);
        tft_.setCursor(2, 36);
        tft_.print("LOC:");
        tft_.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
        tft_.setCursor(60, 36);
        sprintf(timeStr, "%02d:%02d:%02d", timeData.localHours, timeData.localMinutes, timeData.localSeconds);
        tft_.print(timeStr);

        updated = true;
    }

    if( statusChanged(timeData)) {
        // Color code based on signal strength
        uint16_t statusColor = ST77XX_WHITE;
        if (timeData.signalStrength) {
            if (strcmp(timeData.signalStrength, "EXCELLENT") == 0) statusColor = ST77XX_GREEN;
            else if (strcmp(timeData.signalStrength, "GOOD") == 0) statusColor = ST77XX_CYAN;
            else if (strcmp(timeData.signalStrength, "MODERATE") == 0) statusColor = ST77XX_YELLOW;
            else if (strcmp(timeData.signalStrength, "FAIR") == 0) statusColor = ST77XX_MAGENTA;
            else if (strcmp(timeData.signalStrength, "POOR") == 0) statusColor = ST77XX_RED;
            else if (strcmp(timeData.signalStrength, "NO SIGNAL") == 0) statusColor = ST77XX_RED;
        }

        char* lockStr = timeData.validTime ? (char *)"locked" : (char *)"unlocked";

        char statusStr[50];
        if (timeData.signalStrength) {
            sprintf(statusStr, "GPS: %s (%d) %s", timeData.signalStrength, timeData.satelliteCount, lockStr);
        } else {
            sprintf(statusStr, "GPS: UNKNOWN (%d) %s", timeData.satelliteCount, lockStr);
        }

        // Move status line up to fit below time displays
        tft_.fillRect(0, 64, 160, 16, ST77XX_BLACK);
        tft_.setTextColor(statusColor);
        tft_.setTextSize(1);
        tft_.setCursor(0, 66);
        tft_.print(statusStr);

        updated = true;
    }

    if( updated ) {
        // Save last displayed time
        lastDisplayedTime_ = timeData;
    }
}
