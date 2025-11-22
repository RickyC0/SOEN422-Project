#ifndef PLANT1_CONFIG_H
#define PLANT1_CONFIG_H

// Identity
#define PLANT_NAME "Plant 1 (Living Room)"

// Sensor Pins
#define MOISTURE_PIN 4
#define LIGHT_SENSOR_PIN 13 

// Actuator Pins
#define PUMP_PIN 12
#define RGB_RED_PIN 18
#define RGB_GREEN_PIN 23
#define RGB_BLUE_PIN 19

// Pump Calibration Data
#define MOISTURE_DRY 4095
#define MOISTURE_WET 500

// --- THRESHOLDS ---
#define MOISTURE_THRESHOLD_PERCENT 30 // Water below 30%
#define LIGHT_THRESHOLD_LEVEL 3 // Turn on light if level <= 3

// --- TIMING (in Milliseconds) ---
#define PUMP_DURATION_MS 3 * 1000 // Water for 3 seconds
#define WATERING_COOLDOWN_MS 15 * 1000 // Wait 1 hour (60*60*1000) //TODO CHANGE LATER THIS IS ONLY FOR THE DEMO
#define MOISTURE_CHECK_INTERVAL_MS 10 * 1000 // Check soil every 10s //TODO CHANGE LATER THIS IS ONLY FOR THE DEMO
#define LIGHT_CHECK_INTERVAL_MS 10 * 1000 // Check light every 10s //TODO CHANGE LATER THIS IS ONLY FOR THE DEMO
#define LIGHT_PAUSE_INTERVAL_MS 2 * 1000 // Pause lights for 2s to read light level

#endif