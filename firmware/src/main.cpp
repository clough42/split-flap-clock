#include <Arduino.h>
#include <AccelStepper.h>

// Include our new classes
#include "StepperController.h"
#include "TimeDisplay.h"
#include "GPSProcessor.h"
#include "LEDController.h"

// Configuration constants
const int TIMEZONE_OFFSET_HOURS = -7; // Mountain Time (MST = UTC-7, MDT = UTC-6)
const int STEPS_PER_POSITION = 160;   // 200 * 8 microsteps / 10 positions
const int ENABLE_PIN = 8;             // Active low

// Hardware instances - these will be injected into our controllers
AccelStepper hoursTens(AccelStepper::DRIVER, 0, 1);     // Hours tens digit
AccelStepper hoursOnes(AccelStepper::DRIVER, 2, 3);     // Hours ones digit  
AccelStepper minutesTens(AccelStepper::DRIVER, 4, 5);   // Minutes tens digit
AccelStepper minutesOnes(AccelStepper::DRIVER, 6, 7);   // Minutes ones digit

// Controller instances - initialized directly in dependency order
StepperController motorHoursTens(&hoursTens, STEPS_PER_POSITION, "Hours-Tens");
StepperController motorHoursOnes(&hoursOnes, STEPS_PER_POSITION, "Hours-Ones");
StepperController motorMinutesTens(&minutesTens, STEPS_PER_POSITION, "Minutes-Tens");
StepperController motorMinutesOnes(&minutesOnes, STEPS_PER_POSITION, "Minutes-Ones");

// LED controller (no dependencies)
LEDController ledController(13);

// Time display (depends on stepper controllers)
TimeDisplay timeDisplay(&motorHoursTens, &motorHoursOnes, &motorMinutesTens, &motorMinutesOnes);

// GPS processor (depends on timeDisplay and ledController)
GPSProcessor gpsProcessor(TIMEZONE_OFFSET_HOURS, &timeDisplay, &ledController, &Serial3);

void setup() {
    // Initialize serial communication for console output
    Serial.begin(115200);
    Serial.println("GPS Split-Flap Clock Started");

    
    // Initialize components
    ledController.initialize();
    gpsProcessor.initialize();

    // Initialize stepper motor enable pin (active low)
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);  // Enable all motors
    
    Serial.println("Initialization complete");
}

// Background GPS processing - called automatically when Serial3 receives data
void serialEvent3() {
    gpsProcessor.processIncomingData();
}

void loop() {
    // Run stepper motors through the time display service
    timeDisplay.runMotors();
    
    // GPS processing happens automatically in serialEvent3()
}