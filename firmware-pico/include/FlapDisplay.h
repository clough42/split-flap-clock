#ifndef FLAP_DISPLAY_H
#define FLAP_DISPLAY_H

#include <Arduino.h>
#include "StepperController.h"
#include "TimeData.h"

class FlapDisplay {
private:
    StepperController& hoursTens_;
    StepperController& hoursOnes_;
    StepperController& minutesTens_;
    StepperController& minutesOnes_;
    StepperController& secondsTens_;
    StepperController& secondsOnes_;
    int enablePin_;
    int debugPin_;  // Pin 22 for timing measurement

public:
    FlapDisplay(StepperController& hoursTens,
               StepperController& hoursOnes,
               StepperController& minutesTens,
               StepperController& minutesOnes,
               StepperController& secondsTens,
               StepperController& secondsOnes,
               int enablePin,
               int debugPin);

    void initialize();
    void updateTime(const TimeData& timeData);
    void runMotors();
};

#endif // FLAP_DISPLAY_H