#ifndef STEPPER_CONTROLLER_H
#define STEPPER_CONTROLLER_H

#include <AccelStepper.h>

class StepperController {
private:
    AccelStepper* motor_;
    int currentPosition_;  // 0-9
    int stepsPerPosition_;
    String motorName_;
    
public:
    StepperController(AccelStepper* stepperMotor, int stepsPerPos, String name);
    
    void moveToDigit(int targetDigit);
    bool isMoving();
    void run();
    void setCurrentPosition(int position);
};

#endif // STEPPER_CONTROLLER_H