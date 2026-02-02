#include "MechanicalDisplay.h"

MechanicalDisplay::MechanicalDisplay(StepperController& hoursTens,
                                                StepperController& hoursOnes,
                                                StepperController& minutesTens,
                                                StepperController& minutesOnes,
                                                StepperController& secondsTens,
                                                StepperController& secondsOnes,
                                                int enablePin,
                                                int debugPin)
        : hoursTens_(hoursTens), hoursOnes_(hoursOnes),
            minutesTens_(minutesTens), minutesOnes_(minutesOnes), secondsTens_(secondsTens), secondsOnes_(secondsOnes), enablePin_(enablePin), debugPin_(debugPin) {
}

void MechanicalDisplay::initialize() {
    // Initialize shared motor enable pin (active low)
    pinMode(enablePin_, OUTPUT);
    digitalWrite(enablePin_, LOW);  // Enable all motors

    // Initialize debug pin for timing measurement
    pinMode(debugPin_, OUTPUT);
    digitalWrite(debugPin_, LOW);  // Start low

    // Initialize all stepper controllers
    hoursTens_.initialize();
    hoursOnes_.initialize();
    minutesTens_.initialize();
    minutesOnes_.initialize();
    secondsTens_.initialize();
    secondsOnes_.initialize();

    // home the motors
    homeAllMotors();
}

void MechanicalDisplay::homeAllMotors() {
    bool allHomed = false;
    do {
        allHomed = true;
        allHomed &= hoursTens_.runHoming();
        allHomed &= hoursOnes_.runHoming();
        allHomed &= minutesTens_.runHoming();
        allHomed &= minutesOnes_.runHoming();
        allHomed &= secondsTens_.runHoming();
        allHomed &= secondsOnes_.runHoming();
        rp2040.wdt_reset(); // let the watchdog timer know we're still alive
    } while (!allHomed);
}

void MechanicalDisplay::updateTime(const TimeData& timeData) {
    // Only update if time is valid
    if (timeData.validTime) {
        // the motors are running in the other core, so we need to pause it before making updates
        rp2040.idleOtherCore();
        hoursTens_.moveToDigit(timeData.localHours / 10);
        hoursOnes_.moveToDigit(timeData.localHours % 10);
        minutesTens_.moveToDigit(timeData.localMinutes / 10);
        minutesOnes_.moveToDigit(timeData.localMinutes % 10);
        secondsTens_.moveToDigit(timeData.localSeconds / 10);
        secondsOnes_.moveToDigit(timeData.localSeconds % 10);
        rp2040.resumeOtherCore();
    }
}

void MechanicalDisplay::runMotors() {
    digitalWrite(debugPin_, HIGH);  // Start timing measurement
    
    hoursTens_.run();
    hoursOnes_.run();
    minutesTens_.run();
    minutesOnes_.run();
    secondsTens_.run();
    secondsOnes_.run();
    
    digitalWrite(debugPin_, LOW);   // End timing measurement
}

