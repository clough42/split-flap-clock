#ifndef TIME_DISPLAY_H
#define TIME_DISPLAY_H

#include "StepperController.h"

class TimeDisplay {
private:
    StepperController* hoursTens_;
    StepperController* hoursOnes_;
    StepperController* minutesTens_;
    StepperController* minutesOnes_;
    int enablePin_;
    
public:
    TimeDisplay(StepperController* hoursTens, 
                StepperController* hoursOnes,
                StepperController* minutesTens,
                StepperController* minutesOnes,
                int enablePin);
    
    void initialize();
    void updateTime(int hours, int minutes);
    void runMotors();
};

#endif // TIME_DISPLAY_H