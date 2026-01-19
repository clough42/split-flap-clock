#ifndef FLAP_DISPLAY_H
#define FLAP_DISPLAY_H

#include <Arduino.h>
#include "StepperController.h"

class FlapDisplay {
private:
    StepperController* hoursTens_;
    StepperController* hoursOnes_;
    StepperController* minutesTens_;
    StepperController* minutesOnes_;
    int enablePin_;
    int debugPin_;  // Pin 22 for timing measurement
    
public:
    FlapDisplay(StepperController* hoursTens, 
                StepperController* hoursOnes,
                StepperController* minutesTens,
                StepperController* minutesOnes,
                int enablePin,
                int debugPin);
    
    void initialize();
    void updateTime(int hours, int minutes);
    void runMotors();
};

#endif // FLAP_DISPLAY_H