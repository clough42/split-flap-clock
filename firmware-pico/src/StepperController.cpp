#include "StepperController.h"
#include "Configuration.h"

// Private implementation constants
#define MIN_DIGIT              0
#define MAX_DIGIT              9
#define DIGITS_PER_WHEEL       10

StepperController::StepperController(AccelStepper& stepperMotor, int stepsPerPos)
    : motor_(stepperMotor), targetDigit_(0), stepsPerPosition_(stepsPerPos) {
    motor_.setMaxSpeed(MOTOR_MAX_SPEED);
    motor_.setAcceleration(MOTOR_ACCELERATION);
    motor_.setPinsInverted(MOTOR_INVERT_DIRECTION);
}

void StepperController::moveToDigit(int targetDigit) {
    if (targetDigit < MIN_DIGIT || targetDigit > MAX_DIGIT) return;

    // Use our tracked targetDigit_ for logic, and motor_.targetPosition() for step reference
    long currentStepTarget = motor_.targetPosition();
    int currentDigit = targetDigit_;

    int forwardDigits;
    if (targetDigit >= currentDigit) {
        forwardDigits = targetDigit - currentDigit;
    } else {
        forwardDigits = (DIGITS_PER_WHEEL - currentDigit) + targetDigit;
    }

    if (forwardDigits == 0) return; // Already at target

    long newStepTarget = currentStepTarget + (forwardDigits * stepsPerPosition_);
    motor_.moveTo(newStepTarget);
    targetDigit_ = targetDigit;
}

void StepperController::run() {
    motor_.run();
}