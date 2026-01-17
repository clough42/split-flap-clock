#include <Arduino.h>
#include <AccelStepper.h>
#include <IntervalTimer.h>

// Include configuration
#include "Configuration.h"

// Include our new classes
#include "StepperController.h"
#include "TimeDisplay.h"
#include "GPSProcessor.h"
#include "LEDController.h"

// Hardware instances - these will be injected into our controllers
AccelStepper hoursTens(AccelStepper::DRIVER, HOURS_TENS_STEP_PIN, HOURS_TENS_DIR_PIN);
AccelStepper hoursOnes(AccelStepper::DRIVER, HOURS_ONES_STEP_PIN, HOURS_ONES_DIR_PIN);  
AccelStepper minutesTens(AccelStepper::DRIVER, MINUTES_TENS_STEP_PIN, MINUTES_TENS_DIR_PIN);
AccelStepper minutesOnes(AccelStepper::DRIVER, MINUTES_ONES_STEP_PIN, MINUTES_ONES_DIR_PIN);

// Controller instances - initialized directly in dependency order
StepperController motorHoursTens(&hoursTens, STEPS_PER_POSITION);
StepperController motorHoursOnes(&hoursOnes, STEPS_PER_POSITION);
StepperController motorMinutesTens(&minutesTens, STEPS_PER_POSITION);
StepperController motorMinutesOnes(&minutesOnes, STEPS_PER_POSITION);

// LED controller (no dependencies)
LEDController ledController(LED_PIN);

// Time display (depends on stepper controllers)
TimeDisplay timeDisplay(&motorHoursTens, &motorHoursOnes, &motorMinutesTens, &motorMinutesOnes, ENABLE_PIN);

// GPS processor (depends on timeDisplay and ledController)
GPSProcessor gpsProcessor(TIMEZONE_OFFSET_HOURS, &timeDisplay, &ledController, &Serial3);

// Interrupt timer for stepper motor control
IntervalTimer stepperTimer;

// Interrupt service routine - runs motors at consistent intervals
void runSteppersISR() {
    timeDisplay.runMotors();
}

void setup() {
    // Initialize serial communication for console output
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("GPS Split-Flap Clock Started");

    // Initialize components
    ledController.initialize();
    timeDisplay.initialize();
    gpsProcessor.initialize();
    
    // Start interrupt-driven stepper control at 5kHz (200μs interval)
    stepperTimer.begin(runSteppersISR, STEPPER_ISR_INTERVAL_US);  // 200 microseconds = 5000 Hz
    
    Serial.println("Initialization complete - stepper control is interrupt-driven");
}

void loop() {
    // Poll GPS processor for incoming data
    gpsProcessor.processIncomingData();
}