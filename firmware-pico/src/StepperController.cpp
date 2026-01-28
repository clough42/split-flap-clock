
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
    // sanity check to avoig big ugly moves if something goes terribly wrong
    if (targetDigit < MIN_DIGIT || targetDigit > MAX_DIGIT) return;

    // drop out if already at target
    if (targetDigit == targetDigit_) return;

    // reset step count if stationary to avoid step count overflow over long runtimes
    resetStepCountIfStationary();

    // use current target digit to figure how how far to move (forward only)
    int forwardDigits = (targetDigit - targetDigit_);
    if (forwardDigits < 0) forwardDigits += DIGITS_PER_WHEEL;

    // calculate new step target and command move
    long newStepTarget = motor_.targetPosition() + (forwardDigits * stepsPerPosition_);
    motor_.moveTo(newStepTarget);

    // remember where we are going
    targetDigit_ = targetDigit;
}

void StepperController::run() {
    motor_.run();
}

void StepperController::resetStepCountIfStationary() {
    // if the motor isn't moving, this is a good time to reset the current
    // and target positions to zero to avoid step count overflow over long runtimes
    if (motor_.distanceToGo() == 0) {
        motor_.setCurrentPosition(0);
    }
}