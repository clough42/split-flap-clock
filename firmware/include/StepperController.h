#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <AccelStepper.h>

class StepperController {
private:
    AccelStepper* motor_;
    int currentPosition_;  // 0-9
    int stepsPerPosition_;
    
public:
    StepperController(AccelStepper* stepperMotor, int stepsPerPos);
    
    void moveToDigit(int targetDigit);
    void run();
};

#endif // STEPPER_CONTROLLER_H