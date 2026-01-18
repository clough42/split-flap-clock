#include "TFTDisplay.h"
#include "Configuration.h"
#include "LEDController.h"

TFTDisplay::TFTDisplay(int tftCS, int tftDC, int tftRST, int touchCS, int touchIRQ) 
    : tft_(tftCS, tftDC, tftRST), touch_(touchCS, touchIRQ) {
    // Initialize time data
    lastDisplayedTime_ = {0, 0, 0, 0, 0, 0, 0, false};
    lastUpdateTime_ = 0;
}

void TFTDisplay::initialize() {
    // Initialize TFT display
    tft_.begin();
    tft_.setRotation(1); // Landscape orientation (fixed upside-down)
    tft_.fillScreen(ILI9341_BLACK);
    
    // Initialize touch screen
    touch_.begin();
    touch_.setRotation(1);
    
    // Draw initial background
    drawBackground();
    
    Serial.println("TFT Display and touch screen initialized");
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
            newTime.localSeconds != lastDisplayedTime_.localSeconds ||
            newTime.satelliteCount != lastDisplayedTime_.satelliteCount ||
            newTime.validTime != lastDisplayedTime_.validTime);
}

void TFTDisplay::updateTime(const TimeData& timeData) {
    // Only update if time has changed or it's been more than 1 second
    unsigned long currentTime = millis();
    if (!timeChanged(timeData) && (currentTime - lastUpdateTime_) < 1000) {
        return;
    }
    
    lastUpdateTime_ = currentTime;
    
    if (timeData.validTime) {
        // Only clear if switching from invalid to valid state
        if (!lastDisplayedTime_.validTime) {
            // Clear only the "Waiting for GPS..." message area (y=85 to y=140)
            tft_.fillRect(20, 85, 280, 55, ILI9341_BLACK);
            
            // Redraw the time labels that may have been cleared
            tft_.setTextColor(ILI9341_WHITE);
            tft_.setTextSize(2);
            tft_.setCursor(20, 120);
            tft_.println("Local Time:");
        }
        
        // Display UTC time (draw directly over previous, no clearing needed)
        tft_.setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // Set background color
        tft_.setTextSize(3);
        tft_.setCursor(20, 85);
        tft_.printf("%02d:%02d:%02d", timeData.utcHours, timeData.utcMinutes, timeData.utcSeconds);
        
        // Display Local time (draw directly over previous, no clearing needed)
        tft_.setTextColor(ILI9341_GREEN, ILI9341_BLACK);  // Set background color
        tft_.setTextSize(3);
        tft_.setCursor(20, 145);
        tft_.printf("%02d:%02d:%02d", timeData.localHours, timeData.localMinutes, timeData.localSeconds);
        
        // Update status only if changed
        if (!lastDisplayedTime_.validTime) {
            tft_.fillRect(20, 190, 280, 20, ILI9341_BLACK);
            tft_.setTextColor(ILI9341_WHITE);
            tft_.setTextSize(1);
            tft_.setCursor(20, 190);
            tft_.printf("GPS Signal: ACTIVE (%d satellites)", timeData.satelliteCount);
        }
    } else {
        // Display "No GPS" message
        tft_.fillRect(20, 85, 280, 80, ILI9341_BLACK);
        tft_.setTextColor(ILI9341_RED);
        tft_.setTextSize(2);
        tft_.setCursor(20, 110);
        tft_.println("Waiting for GPS...");
        
        tft_.setTextColor(ILI9341_WHITE);
        tft_.setTextSize(1);
        tft_.setCursor(20, 190);
        tft_.println("GPS Signal: SEARCHING");
    }
    
    lastDisplayedTime_ = timeData;
}

void TFTDisplay::handleTouch() {
    if (touch_.tirqTouched()) {
        if (touch_.touched()) {
            TS_Point point = touch_.getPoint();
            // Handle touch events here - for now just print coordinates
            Serial.printf("Touch detected: X=%d, Y=%d\n", point.x, point.y);
        }
    }
}

bool TFTDisplay::isTouchPressed() {
    return touch_.touched();
}