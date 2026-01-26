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

// TFT Display Pin Assignments (ILI9341) - Default SPI0 pins
#define TFT_MISO_PIN            16       // Hardware SPI MISO (default)
#define TFT_CS_PIN              17       // TFT chip select
#define TFT_SCK_PIN             18       // Hardware SPI SCK
#define TFT_MOSI_PIN            19       // Hardware SPI MOSI (default)
#define TFT_DC_PIN              20       // TFT data/command
#define TFT_RST_PIN             255      // TFT reset (255 = not used)

// GPS Serial Pin Assignments
#define GPS_TX_PIN              0
#define GPS_RX_PIN              1

// Stepper Motor Pin Assignments
#define HOURS_TENS_STEP_PIN     2
#define HOURS_TENS_DIR_PIN      3
#define HOURS_ONES_STEP_PIN     4
#define HOURS_ONES_DIR_PIN      5
#define MINUTES_TENS_STEP_PIN   6
#define MINUTES_TENS_DIR_PIN    7
#define MINUTES_ONES_STEP_PIN   8
#define MINUTES_ONES_DIR_PIN    9
#define SECONDS_TENS_STEP_PIN   10
#define SECONDS_TENS_DIR_PIN    11
#define SECONDS_ONES_STEP_PIN   12
#define SECONDS_ONES_DIR_PIN    13

// Control Pins
#define ENABLE_PIN              14      // Active low - enables all stepper motors
#define LED_PIN                 LED_BUILTIN  // Use Pico built-in LED (GPIO 25)
#define DEBUG_PIN               15      // Debug timing pin for runMotors() measurement
#define TIMEZONE_BUTTON_PIN     21      // Timezone offset button (active low with internal pullup)

// Button Debounce Configuration
#define BUTTON_DEBOUNCE_MS      200     // Debounce time in milliseconds

// ============================================================================
// MOTOR CONFIGURATION
// ============================================================================
#define STEPS_PER_POSITION      160     // 200 * 8 microsteps / 10 positions
#define MOTOR_MAX_SPEED         1000    // Steps per second
#define MOTOR_ACCELERATION      10000   // Steps per second²

#endif // CONFIGURATION_H