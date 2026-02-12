#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// ============================================================================
// COMMUNICATION SETTINGS
// ============================================================================
#define SERIAL_BAUD_RATE        115200  // Console serial port baud rate
#define GPS_BAUD_RATE           9600    // GPS serial port baud rate

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

// TFT Display Pin Assignments
#define TFT_MISO_PIN            16       // Hardware SPI MISO
#define TFT_CS_PIN              17       // TFT chip select
#define TFT_SCK_PIN             18       // Hardware SPI SCK
#define TFT_MOSI_PIN            19       // Hardware SPI MOSI
#define TFT_DC_PIN              20       // TFT data/command
#define TFT_RST_PIN             21       // TFT reset

// GPS Serial Pin Assignments
#define GPS_TX_PIN              0
#define GPS_RX_PIN              1

// Stepper Motor Pin Assignments
#define HOURS_TENS_STEP_PIN     2
#define HOURS_TENS_DIR_PIN      15
#define HOURS_TENS_HOME_PIN     3
#define HOURS_ONES_STEP_PIN     4
#define HOURS_ONES_DIR_PIN      15
#define HOURS_ONES_HOME_PIN     5
#define MINUTES_TENS_STEP_PIN   6
#define MINUTES_TENS_DIR_PIN    15
#define MINUTES_TENS_HOME_PIN   7
#define MINUTES_ONES_STEP_PIN   8
#define MINUTES_ONES_DIR_PIN    15
#define MINUTES_ONES_HOME_PIN   9
#define SECONDS_TENS_STEP_PIN   10
#define SECONDS_TENS_DIR_PIN    15
#define SECONDS_TENS_HOME_PIN   11
#define SECONDS_ONES_STEP_PIN   12
#define SECONDS_ONES_DIR_PIN    15
#define SECONDS_ONES_HOME_PIN   13


// Control Pins
#define ENABLE_PIN              22      // Active low - enables all stepper motors
#define LED_PIN                 LED_BUILTIN  // Use Pico built-in LED
#define DEBUG_PIN               28      // Debug timing pin for runMotors() measurement
#define TIMEZONE_BUTTON_PIN     26      // Timezone offset button

// Button Debounce Configuration
#define BUTTON_DEBOUNCE_MS      50     // Debounce time in milliseconds
#define BUTTON_LONG_PRESS_MS    2000   // Long press time in milliseconds

// Watchdog Timer Configuration
#define WATCHDOG_TIMEOUT_MS     4000    // Watchdog timer timeout in milliseconds

// ============================================================================
// MOTOR CONFIGURATION
// ============================================================================
#define STEPS_PER_POSITION      160     // 200 * 8 microsteps / 10 positions
#define MOTOR_HOMING_SPEED      5000     // Steps per second during homing
#define MOTOR_MAX_SPEED         5000    // Steps per second
#define MOTOR_ACCELERATION      50000   // Steps per second²
#define MOTOR_INVERT_DIRECTION  true    // Set to true to invert stepper direction, false for normal

// Per-motor homing offset (number of steps to continue moving after the home switch is triggered)
#define HOMING_OFFSET_STEPS_HOURS_TENS      110
#define HOMING_OFFSET_STEPS_HOURS_ONES      90
#define HOMING_OFFSET_STEPS_MINUTES_TENS    90
#define HOMING_OFFSET_STEPS_MINUTES_ONES    90
#define HOMING_OFFSET_STEPS_SECONDS_TENS    80
#define HOMING_OFFSET_STEPS_SECONDS_ONES    100


#endif // CONFIGURATION_H