#ifndef PLANT1_CONFIG_H
#define PLANT1_CONFIG_H

/**
 * @brief Configuration for PLANT NODE 1.
 * This file defines the hardware wiring, calibration data, and logical behavior
 * specific to the physical plant setup in the "Living Room".
 */

// --- IDENTITY ---
#define PLANT_NAME "Plant 1 (Living Room)"

// --- SENSOR PINS ---
/**
 * @brief Analog Input Pins.
 * @warning Must use ADC1 pins (32, 33, 34, 35, 36, 39).
 * Pins 0, 2, 4, 12-15, 25-27 are ADC2 and CANNOT be used while Wi-Fi is active.
 */
#define MOISTURE_PIN 34     // Input Only pin (Safe)
#define LIGHT_SENSOR_PIN 35 // Input Only pin (Safe)

// --- ACTUATOR PINS ---
/**
 * @brief Digital Output Pins.
 * Any GPIO can be used for output, but avoid strapping pins (0, 2, 12, 15) if possible
 * to prevent boot issues.
 */
#define PUMP_PIN 12
#define RGB_RED_PIN 18
#define RGB_GREEN_PIN 23
#define RGB_BLUE_PIN 19

// --- SERIAL COMMUNICATION ---
#define SERIAL_BAUD_RATE 115200

// --- SENSOR CALIBRATION ---
/**
 * @brief Capacitive Moisture Sensor Limits.
 * Determined experimentally by placing sensor in air (Dry) and water (Wet).
 */
#define MOISTURE_DRY 4095 // Maximum resistance (Air)
#define MOISTURE_WET 500  // Minimum resistance (Submerged)

// --- LOGIC THRESHOLDS ---
/**
 * @brief Trigger points for automation.
 */
#define MOISTURE_THRESHOLD_PERCENT 30 // If moisture < 30%, pump activates
#define LIGHT_THRESHOLD_LEVEL 2       // If Status <= 2 (Normal/Dim), grow light activates

// --- TIMING CONFIGURATION ---
/**
 * @brief System timing variables (in Milliseconds).
 * Using math (3 * 1000) makes it easier to read as "3 seconds".
 */
#define PUMP_DURATION_MS 3 * 1000            // Run pump for 3 seconds per watering event
#define WATERING_COOLDOWN_MS 1 * 1000        // Wait before allowing another watering (Demo: 1s / Real: 3600000)
#define MOISTURE_CHECK_INTERVAL_MS 30 * 1000 // How often to read soil sensor
#define LIGHT_CHECK_INTERVAL_MS 30 * 1000    // How often to read light sensor
#define LIGHT_PAUSE_INTERVAL_MS 2 * 1000     // "Peeking" duration: Turn off LED for 2s to measure ambient light
#define UPLOAD_TIMER 5 * 1000                // Push sensor data to Firebase every 5s

// --- WI-FI SETTINGS ---
#define WIFI_DELAY_MS 500 // Wait time between connection attempts

// --- CAMERA HARDWARE (Freenove WROVER) ---
/**
 * @brief Pin definitions for the Freenove ESP32-WROVER Camera Board.
 * Do not change these unless using a different physical board (e.g. AI-Thinker).
 */
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// --- FIREBASE SETTINGS ---
#define SERVER_PICTURE_RESPONSE_TIMEOUT_MS 10 * 1000 // Give server 10s to accept image
#define CAMERA_UPLOAD_INTERVAL_MS 60 * 1000          // Take photo every 60 seconds

#endif // PLANT1_CONFIG_H