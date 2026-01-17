#include "StepperController.h"

StepperController::StepperController(AccelStepper* stepperMotor, int stepsPerPos, String name) 
    : motor_(stepperMotor), currentPosition_(0), stepsPerPosition_(stepsPerPos), motorName_(name) {
    motor_->setMaxSpeed(1000);        // Steps per second
    motor_->setAcceleration(2000);    // Steps per second²
}

void StepperController::moveToDigit(int targetDigit) {
    if (targetDigit < 0 || targetDigit > 9) return;
    
    // Calculate forward-only distance
    int forwardSteps;
    if (targetDigit >= currentPosition_) {
        // Normal forward movement
        forwardSteps = (targetDigit - currentPosition_) * stepsPerPosition_;
    } else {
        // Wrap around (go past 9 back to target)
        forwardSteps = ((10 - currentPosition_) + targetDigit) * stepsPerPosition_;
    }
    
    if (forwardSteps > 0) {
        motor_->move(forwardSteps);
    }
    currentPosition_ = targetDigit;
}

bool StepperController::isMoving() { 
    return motor_->isRunning(); 
}

void StepperController::run() {
    motor_->run();
}

void StepperController::setCurrentPosition(int position) {
    currentPosition_ = position;
}