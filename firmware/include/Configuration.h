#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

// Stepper Motor Pin Assignments
#define HOURS_TENS_STEP_PIN     0
#define HOURS_TENS_DIR_PIN      1
#define HOURS_ONES_STEP_PIN     2
#define HOURS_ONES_DIR_PIN      3
#define MINUTES_TENS_STEP_PIN   4
#define MINUTES_TENS_DIR_PIN    5
#define MINUTES_ONES_STEP_PIN   6
#define MINUTES_ONES_DIR_PIN    7

// Control Pins
#define ENABLE_PIN              18      // Active low - enables all stepper motors
#define LED_PIN                 23      // External LED for GPS status (pin 13 reserved for SPI)
#define DEBUG_PIN               22      // Debug timing pin for runMotors() measurement

// TFT Display Pin Assignments (ILI9341)
#define TFT_CS_PIN              10      // TFT chip select
#define TFT_DC_PIN              9       // TFT data/command
#define TFT_RST_PIN             255     // TFT reset (255 = not used)
#define TFT_MOSI_PIN            11      // Hardware SPI MOSI
#define TFT_MISO_PIN            12      // Hardware SPI MISO  
#define TFT_SCK_PIN             13      // Hardware SPI SCK (must be pin 13 for Teensy 4.0)

// Touch Screen Pin Assignments
#define TOUCH_CS_PIN            8       // Touch chip select
#define TOUCH_IRQ_PIN           17      // Touch interrupt (custom)

// ============================================================================
// MOTOR CONFIGURATION
// ============================================================================
#define STEPS_PER_POSITION      160     // 200 * 8 microsteps / 10 positions
#define MOTOR_MAX_SPEED         1000    // Steps per second
#define MOTOR_ACCELERATION      2000    // Steps per second²

// ============================================================================
// COMMUNICATION SETTINGS
// ============================================================================
#define SERIAL_BAUD_RATE        115200  // Console serial port baud rate
#define GPS_BAUD_RATE          9600    // GPS serial port baud rate

// ============================================================================
// SYSTEM TIMING
// ============================================================================
#define STEPPER_ISR_INTERVAL_US 200     // 200 microseconds = 5kHz interrupt rate

// ============================================================================
// APPLICATION CONFIGURATION
// ============================================================================
#define TIMEZONE_OFFSET_HOURS   -7      // Mountain Time (MST = UTC-7, MDT = UTC-6)

#endif // CONFIGURATION_H