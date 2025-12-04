#include <Arduino.h>
#include "FirebaseManager.h"
#include "WiFi.h"

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
// Give this plant a unique ID in the database (e.g., "plant1")
FirebaseManager cloud("plant1");

unsigned long lastUploadTime = 0;

void setup() {
    Serial.begin(115200);
    
    // Connect to Wi-Fi FIRST
    Serial.print("Connecting to Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");

    myPlant.begin();
    cloud.begin();
}

void loop() {
    myPlant.update();

    // --- CLOUD UPLOAD (Every 5 Seconds) ---
    if (millis() - lastUploadTime > 5000) {
        lastUploadTime = millis();

        // Gather data from the Plant getters
        cloud.sendData(
            myPlant.getMoisturePercent(),
            myPlant.getLightStatus(),
            myPlant.isPumpRunning(),
            myPlant.isLightOn()
        );
    }
}