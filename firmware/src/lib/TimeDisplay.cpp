#include "TimeDisplay.h"

TimeDisplay::TimeDisplay(StepperController* hoursTens, 
                        StepperController* hoursOnes,
                        StepperController* minutesTens,
                        StepperController* minutesOnes)
    : hoursTens_(hoursTens), hoursOnes_(hoursOnes), 
      minutesTens_(minutesTens), minutesOnes_(minutesOnes) {
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