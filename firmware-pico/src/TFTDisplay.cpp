

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


void TFTDisplay::showHomingScreen() {
    drawBackground();
    tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK); // Orange text
    tft_.setTextSize(2);
    // Centered horizontally, vertically
    int16_t x1, y1;
    uint16_t w, h;
    tft_.getTextBounds("HOMING", 0, 0, &x1, &y1, &w, &h);
    int x = (160 - w) / 2;
    int y = (80 - h) / 2;
    tft_.setCursor(x, y);
    tft_.print("HOMING");

    needsClear_ = true;
}

void TFTDisplay::showWaitingForGpsScreen() {
    drawBackground();
    tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK); // Orange text
    tft_.setTextSize(2); // Match HOMING message size
    // Centered horizontally, vertically
    const char* msg1 = "WAITING";
    const char* msg2 = "FOR GPS";
    int16_t x1, y1;
    uint16_t w, h;
    // First line: WAITING
    tft_.getTextBounds(msg1, 0, 0, &x1, &y1, &w, &h);
    int yStart = (tft_.height() - (h * 2 + 8)) / 2; // 8px gap between lines
    tft_.setCursor((tft_.width() - w) / 2, yStart);
    tft_.print(msg1);
    // Second line: FOR GPS
    tft_.getTextBounds(msg2, 0, 0, &x1, &y1, &w, &h);
    tft_.setCursor((tft_.width() - w) / 2, yStart + h + 8);
    tft_.print(msg2);

    needsClear_ = true;
}

void TFTDisplay::drawBackground() {
    if (needsClear_) {
        tft_.fillScreen(ST77XX_BLACK);
        needsClear_ = false;
    }
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
        drawBackground(); // will only draw if needed

        lastUpdateTime_ = currentTime;

        // Define a custom gray color (RGB565: 0x8410)
        const uint16_t GRAY = 0x8410;

        // Draw 12H / 24H indicator at the top
        tft_.setTextSize(1);
        int indicatorY = 2;
        int indicatorX = 52; // Centered for 160px wide screen
        // Draw "12H / 24H" with active mode white, inactive gray
        if (timeData.is24HourFormat) {
            tft_.setTextColor(GRAY, ST77XX_BLACK); tft_.setCursor(indicatorX, indicatorY); tft_.print("12H");
            tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK); tft_.setCursor(indicatorX + 24, indicatorY); tft_.print("/");
            tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK); tft_.setCursor(indicatorX + 36, indicatorY); tft_.print("24H");
            // No AM/PM in 24h mode
            tft_.fillRect(indicatorX + 80, indicatorY, 24, 10, ST77XX_BLACK); // Clear area
        } else {
            tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK); tft_.setCursor(indicatorX, indicatorY); tft_.print("12H");
            tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK); tft_.setCursor(indicatorX + 24, indicatorY); tft_.print("/");
            tft_.setTextColor(GRAY, ST77XX_BLACK); tft_.setCursor(indicatorX + 36, indicatorY); tft_.print("24H");
            // Show AM/PM
            tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
            tft_.setCursor(indicatorX + 80, indicatorY);
            tft_.print(timeData.isPm ? "PM" : "AM");
        }

        // Move times down to make room for indicator
        int utcY = 18;    // Move UTC time down 2 pixels
        int localY = 45;  // Move local time up 1 pixel

        // Display UTC time
        tft_.setTextColor(GRAY, ST77XX_BLACK);
        tft_.setTextSize(2);
        tft_.setCursor(2, utcY);
        tft_.print("UTC:");
        tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft_.setCursor(60, utcY);
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d:%02d", timeData.utcHours, timeData.utcMinutes, timeData.utcSeconds);
        tft_.print(timeStr);

        // Display Local time
        tft_.setTextColor(GRAY, ST77XX_BLACK);
        tft_.setTextSize(2);
        tft_.setCursor(2, localY);
        tft_.print("LOC:");
        tft_.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        tft_.setCursor(60, localY);
        sprintf(timeStr, "%02d:%02d:%02d", timeData.localHours, timeData.localMinutes, timeData.localSeconds);
        tft_.print(timeStr);

        updated = true;
    }

    if( statusChanged(timeData)) {
        drawBackground(); // will only draw if needed

        // Color code based on signal strength (red/blue swapped for BGR display)
        uint16_t statusColor = ST77XX_WHITE;
        if (timeData.signalStrength) {
            if (strcmp(timeData.signalStrength, "EXCELLENT") == 0) statusColor = 0x1F04; // true GREEN
            else if (strcmp(timeData.signalStrength, "GOOD") == 0) statusColor = 0xF81F; // true CYAN
            else if (strcmp(timeData.signalStrength, "MODERATE") == 0) statusColor = 0xE0FF; // true YELLOW
            else if (strcmp(timeData.signalStrength, "FAIR") == 0) statusColor = 0xF800; // true MAGENTA
            else if (strcmp(timeData.signalStrength, "POOR") == 0) statusColor = 0x001F; // true RED
            else if (strcmp(timeData.signalStrength, "NO SIGNAL") == 0) statusColor = 0x001F; // true RED
        }

        char* lockStr = timeData.validTime ? (char *)"locked" : (char *)"unlocked";

        char statusStr[50];
        if (timeData.signalStrength) {
            sprintf(statusStr, "GPS: %s (%d) %s", timeData.signalStrength, timeData.satelliteCount, lockStr);
        } else {
            sprintf(statusStr, "GPS: UNKNOWN (%d) %s", timeData.satelliteCount, lockStr);
        }

        // Move status line up 1 more pixel
        tft_.fillRect(0, 71, 160, 16, ST77XX_BLACK);
        tft_.setTextColor(statusColor);
        tft_.setTextSize(1);
        tft_.setCursor(0, 73);
        tft_.print(statusStr);

        updated = true;
    }

    if( updated ) {
        // Save last displayed time
        lastDisplayedTime_ = timeData;
    }
}
