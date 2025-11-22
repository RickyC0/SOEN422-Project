#include <Arduino.h>
#include "SystemConfig.h" 
#include "Plant.h"

Plant myPlant(
    PUMP_PIN, 
    MOISTURE_PIN, 
    LIGHT_SENSOR_PIN, 
    RGB_RED_PIN, 
    RGB_GREEN_PIN, 
    RGB_BLUE_PIN,
    // Calibration
    MOISTURE_DRY,
    MOISTURE_WET,
    // Settings
    MOISTURE_THRESHOLD_PERCENT,
    LIGHT_THRESHOLD_LEVEL,
    PUMP_DURATION_MS,
    WATERING_COOLDOWN_MS,
    MOISTURE_CHECK_INTERVAL_MS,
    LIGHT_CHECK_INTERVAL_MS,
    LIGHT_PAUSE_INTERVAL_MS
);

void setup() {
    Serial.begin(115200);
    myPlant.begin();
}

void loop() {
    myPlant.update();
}