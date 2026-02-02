#include <Arduino.h>
#include <SPI.h>

// Include configuration
#include "Configuration.h"

// Include our classes
#include "StepperController.h"
#include "MechanicalDisplay.h"
#include "GPSProcessor.h"
#include "LEDController.h"
#include "TFTDisplay.h"
#include "DebouncedButton.h"
#include "ConfigPersistence.h"

// Configuration persistence
ConfigPersistence configPersistence;

// Hardware instances - these will be injected into our controllers
AccelStepper hoursTens(AccelStepper::DRIVER, HOURS_TENS_STEP_PIN, HOURS_TENS_DIR_PIN);
AccelStepper hoursOnes(AccelStepper::DRIVER, HOURS_ONES_STEP_PIN, HOURS_ONES_DIR_PIN);  
AccelStepper minutesTens(AccelStepper::DRIVER, MINUTES_TENS_STEP_PIN, MINUTES_TENS_DIR_PIN);
AccelStepper minutesOnes(AccelStepper::DRIVER, MINUTES_ONES_STEP_PIN, MINUTES_ONES_DIR_PIN);
AccelStepper secondsTens(AccelStepper::DRIVER, SECONDS_TENS_STEP_PIN, SECONDS_TENS_DIR_PIN);
AccelStepper secondsOnes(AccelStepper::DRIVER, SECONDS_ONES_STEP_PIN, SECONDS_ONES_DIR_PIN);

// Controller instances - initialized directly in dependency order
StepperController motorHoursTens(hoursTens, STEPS_PER_POSITION, HOURS_TENS_HOME_PIN, HOMING_OFFSET_STEPS_HOURS_TENS);
StepperController motorHoursOnes(hoursOnes, STEPS_PER_POSITION, HOURS_ONES_HOME_PIN, HOMING_OFFSET_STEPS_HOURS_ONES);
StepperController motorMinutesTens(minutesTens, STEPS_PER_POSITION, MINUTES_TENS_HOME_PIN, HOMING_OFFSET_STEPS_MINUTES_TENS);
StepperController motorMinutesOnes(minutesOnes, STEPS_PER_POSITION, MINUTES_ONES_HOME_PIN, HOMING_OFFSET_STEPS_MINUTES_ONES);
StepperController motorSecondsTens(secondsTens, STEPS_PER_POSITION, SECONDS_TENS_HOME_PIN, HOMING_OFFSET_STEPS_SECONDS_TENS);
StepperController motorSecondsOnes(secondsOnes, STEPS_PER_POSITION, SECONDS_ONES_HOME_PIN, HOMING_OFFSET_STEPS_SECONDS_ONES);

// LED controller (no dependencies)
LEDController ledController(LED_PIN);

// Mechanical display (depends on stepper controllers)
MechanicalDisplay mechanicalDisplay(motorHoursTens, motorHoursOnes, motorMinutesTens, motorMinutesOnes, motorSecondsTens, motorSecondsOnes, ENABLE_PIN, DEBUG_PIN);

TFTDisplay tftDisplay(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

// GPS processor with real dependencies
GPSProcessor gpsProcessor(configPersistence, mechanicalDisplay, tftDisplay, ledController, Serial1);

// Timezone button with debouncing
DebouncedButton configButton(TIMEZONE_BUTTON_PIN, BUTTON_DEBOUNCE_MS, BUTTON_LONG_PRESS_MS);

void setup() {
    // initialize the hardware watchdog timer
    rp2040.wdt_begin(WATCHDOG_TIMEOUT_MS);

    // Initialize serial communication for console output
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("GPS Mechanical Clock Started");
    
    // Select pins for GPS UART
    Serial1.setRX(GPS_RX_PIN);
    Serial1.setTX(GPS_TX_PIN);

    // Select pins for TFT SPI
    SPI.setRX(TFT_MISO_PIN);
    SPI.setTX(TFT_MOSI_PIN);
    SPI.setSCK(TFT_SCK_PIN);

    // Initialize everything
    configPersistence.initialize();
    configButton.initialize();
    ledController.initialize();
    mechanicalDisplay.initialize();
    tftDisplay.initialize();
    gpsProcessor.initialize();
    
    // release core 1 for motor control
    rp2040.fifo.push(1);
}

void setup1() {
    // wait for signal from core 0
    rp2040.fifo.pop();
}

void loop() {
    // Handle config button
    configButton.update();
    if (configButton.wasShortPressed()) {
        gpsProcessor.incrementTimezoneOffset();
    }
    if (configButton.wasLongPressed()) {
        gpsProcessor.toggleTimeFormat();
    }

    // Process GPS data using GPSProcessor
    gpsProcessor.processIncomingData();

    // let the watchdog know we're alive
    rp2040.wdt_reset();
}

void loop1() {
    // Service the stepper motors
    mechanicalDisplay.runMotors();
}