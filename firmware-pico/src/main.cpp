#include <Arduino.h>

// Include configuration and classes
#include "Configuration.h"
#include "GPSProcessor.h"
#include "FlapDisplay.h"
#include "LEDController.h"
#include "TFTDisplay.h"

// Create dummy objects
LEDController ledController(LED_PIN);
FlapDisplay flapDisplay;
TFTDisplay tftDisplay(10, 9, 8, 7, 6);  // Dummy pins

// GPS processor with real dependencies
GPSProcessor gpsProcessor(TIMEZONE_OFFSET_HOURS, &flapDisplay, &ledController, &Serial1);

void setup() {
    // Initialize console serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("GPS Split-Flap Clock - RP2040 Port with GPSProcessor");
    
    // Initialize LED controller
    ledController.initialize();
    
    // Set TFT display controller
    gpsProcessor.setDisplayController(&tftDisplay);
    
    // Initialize GPS processor
    gpsProcessor.initialize();
    
    Serial.println("GPSProcessor initialized. LED will toggle when GPS gets valid time.");
}

void loop() {
    // Process GPS data using GPSProcessor
    gpsProcessor.processIncomingData();
    
    // Heartbeat
    static unsigned long lastBeat = 0;
    if (millis() - lastBeat >= 10000) {
        Serial.println("System running...");
        lastBeat = millis();
    }
}