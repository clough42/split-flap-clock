#include "StepperController.h"
#include "Configuration.h"

// Private implementation constants
#define MIN_DIGIT              0
#define MAX_DIGIT              9
#define DIGITS_PER_WHEEL       10

StepperController::StepperController(AccelStepper& stepperMotor, int stepsPerPos)
    : motor_(stepperMotor), currentPosition_(0), stepsPerPosition_(stepsPerPos) {
    motor_.setMaxSpeed(MOTOR_MAX_SPEED);
    motor_.setAcceleration(MOTOR_ACCELERATION);
}

void StepperController::moveToDigit(int targetDigit) {
    if (targetDigit < MIN_DIGIT || targetDigit > MAX_DIGIT) return;
    
    // Calculate forward-only distance
    int forwardSteps;
    if (targetDigit >= currentPosition_) {
        // Normal forward movement
        forwardSteps = (targetDigit - currentPosition_) * stepsPerPosition_;
    } else {
        // Wrap around (go past 9 back to target)
        forwardSteps = ((DIGITS_PER_WHEEL - currentPosition_) + targetDigit) * stepsPerPosition_;
    }
    
    if (forwardSteps > 0) {
        motor_.move(forwardSteps);
    }
    currentPosition_ = targetDigit;
}

void StepperController::run() {
    motor_.run();
}