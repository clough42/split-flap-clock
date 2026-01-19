#include "FlapDisplay.h"

FlapDisplay::FlapDisplay(StepperController* hoursTens, 
                        StepperController* hoursOnes,
                        StepperController* minutesTens,
                        StepperController* minutesOnes,
                        int enablePin,
                        int debugPin)
    : hoursTens_(hoursTens), hoursOnes_(hoursOnes), 
      minutesTens_(minutesTens), minutesOnes_(minutesOnes), enablePin_(enablePin), debugPin_(debugPin) {
}

void FlapDisplay::initialize() {
    // Initialize shared motor enable pin (active low)
    pinMode(enablePin_, OUTPUT);
    digitalWrite(enablePin_, LOW);  // Enable all motors
    
    // Initialize debug pin for timing measurement
    pinMode(debugPin_, OUTPUT);
    digitalWrite(debugPin_, LOW);  // Start low
}

void FlapDisplay::updateTime(int hours, int minutes) {
    hoursTens_->moveToDigit(hours / 10);
    hoursOnes_->moveToDigit(hours % 10);
    minutesTens_->moveToDigit(minutes / 10);
    minutesOnes_->moveToDigit(minutes % 10);
}

void FlapDisplay::runMotors() {
    digitalWrite(debugPin_, HIGH);  // Start timing measurement
    
    hoursTens_->run();
    hoursOnes_->run();
    minutesTens_->run();
    minutesOnes_->run();
    
    digitalWrite(debugPin_, LOW);   // End timing measurement
}