#include "TimeDisplay.h"

TimeDisplay::TimeDisplay(StepperController* hoursTens, 
                        StepperController* hoursOnes,
                        StepperController* minutesTens,
                        StepperController* minutesOnes,
                        int enablePin)
    : hoursTens_(hoursTens), hoursOnes_(hoursOnes), 
      minutesTens_(minutesTens), minutesOnes_(minutesOnes), enablePin_(enablePin) {
}

void TimeDisplay::initialize() {
    // Initialize shared motor enable pin (active low)
    pinMode(enablePin_, OUTPUT);
    digitalWrite(enablePin_, LOW);  // Enable all motors
}

void TimeDisplay::updateTime(int hours, int minutes) {
    hoursTens_->moveToDigit(hours / 10);
    hoursOnes_->moveToDigit(hours % 10);
    minutesTens_->moveToDigit(minutes / 10);
    minutesOnes_->moveToDigit(minutes % 10);
}

void TimeDisplay::runMotors() {
    hoursTens_->run();
    hoursOnes_->run();
    minutesTens_->run();
    minutesOnes_->run();
}