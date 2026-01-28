#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <AccelStepper.h>

class StepperController {
private:
    AccelStepper& motor_;
    int targetDigit_;  // 0-9, the digit we are moving toward
    int stepsPerPosition_;
    int homingPin_;

    void resetStepCountIfStationary();
    void home();

public:
    StepperController(AccelStepper& stepperMotor, int stepsPerPos, int homingPin);

    void initialize();
    void moveToDigit(int targetDigit);
    void run();
};

#endif // STEPPER_CONTROLLER_H