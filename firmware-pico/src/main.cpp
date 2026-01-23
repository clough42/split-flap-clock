#include <Arduino.h>

// Include configuration and classes
#include "Configuration.h"
#include "GPSProcessor.h"
#include "FlapDisplay.h"
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

TFTDisplay tftDisplay(10, 9, 8, 7, 6);  // Dummy pins

// GPS processor with real dependencies
GPSProcessor gpsProcessor(TIMEZONE_OFFSET_HOURS, &flapDisplay, &ledController, &Serial1);

void setup() {
    // Initialize console serial
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("GPS Split-Flap Clock - RP2040 Port with GPSProcessor");
    
    // Initialize LED controller
    ledController.initialize();
    flapDisplay.initialize();
    
    // Set TFT display controller
    gpsProcessor.setDisplayController(&tftDisplay);
    
    // Initialize GPS processor
    gpsProcessor.initialize();
    
    Serial.println("GPSProcessor initialized. LED will toggle when GPS gets valid time.");
}

void loop() {
    // Process GPS data using GPSProcessor
    gpsProcessor.processIncomingData();

    flapDisplay.runMotors();

}