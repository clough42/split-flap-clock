#include <Arduino.h>
#include <SPI.h>

// Include configuration
#include "Configuration.h"

// Include our classes
#include "StepperController.h"
#include "FlapDisplay.h"
#include "GPSProcessor.h"
#include "LEDController.h"
#include "TFTDisplay.h"

// Hardware instances - these will be injected into our controllers
AccelStepper hoursTens(AccelStepper::DRIVER, HOURS_TENS_STEP_PIN, HOURS_TENS_DIR_PIN);
AccelStepper hoursOnes(AccelStepper::DRIVER, HOURS_ONES_STEP_PIN, HOURS_ONES_DIR_PIN);  
AccelStepper minutesTens(AccelStepper::DRIVER, MINUTES_TENS_STEP_PIN, MINUTES_TENS_DIR_PIN);
AccelStepper minutesOnes(AccelStepper::DRIVER, MINUTES_ONES_STEP_PIN, MINUTES_ONES_DIR_PIN);
AccelStepper secondsTens(AccelStepper::DRIVER, SECONDS_TENS_STEP_PIN, SECONDS_TENS_DIR_PIN);
AccelStepper secondsOnes(AccelStepper::DRIVER, SECONDS_ONES_STEP_PIN, SECONDS_ONES_DIR_PIN);

// Controller instances - initialized directly in dependency order
StepperController motorHoursTens(&hoursTens, STEPS_PER_POSITION);
StepperController motorHoursOnes(&hoursOnes, STEPS_PER_POSITION);
StepperController motorMinutesTens(&minutesTens, STEPS_PER_POSITION);
StepperController motorMinutesOnes(&minutesOnes, STEPS_PER_POSITION);
StepperController motorSecondsTens(&secondsTens, STEPS_PER_POSITION);
StepperController motorSecondsOnes(&secondsOnes, STEPS_PER_POSITION);

// LED controller (no dependencies)
LEDController ledController(LED_PIN);

// Flap display (depends on stepper controllers)
FlapDisplay flapDisplay(&motorHoursTens, &motorHoursOnes, &motorMinutesTens, &motorMinutesOnes, &motorSecondsTens, &motorSecondsOnes, ENABLE_PIN, DEBUG_PIN);

TFTDisplay tftDisplay(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

// GPS processor with real dependencies
GPSProcessor gpsProcessor(TIMEZONE_OFFSET_HOURS, &flapDisplay, &ledController, &Serial1);

void setup() {
    // Initialize serial communication for console output
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("GPS Split-Flap Clock Started");
    
    // Select pins for GPS UART
    Serial1.setRX(GPS_RX_PIN);
    Serial1.setTX(GPS_TX_PIN);

    // Select pins for TFT SPI
    SPI.setRX(TFT_MISO_PIN);
    SPI.setTX(TFT_MOSI_PIN);
    SPI.setSCK(TFT_SCK_PIN);

    // Initialize LED controller
    ledController.initialize();
    flapDisplay.initialize();
    tftDisplay.initialize();
    gpsProcessor.initialize();
    
    // Set TFT display controller
    gpsProcessor.setDisplayController(&tftDisplay);
    
    Serial.println("GPSProcessor initialized. LED will toggle when GPS gets valid time.");
}

void loop() {
    // Process GPS data using GPSProcessor
    gpsProcessor.processIncomingData();

    // Handle touch screen interactions
    tftDisplay.handleTouch();

    // Service the stepper motors
    flapDisplay.runMotors();

}