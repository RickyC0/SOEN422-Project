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

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWi-Fi Connected!");
}

void setup() {
    Serial.begin(115200);
    
    // Connect to Wi-Fi FIRST
    initWiFi();

    myPlant.begin();
    cloud.begin();
}

void loop() {
    // Reconnect Wi-Fi if disconnected at anytime during runtime
    if (WiFi.status() != WL_CONNECTED) {
        initWiFi();
    }

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

    bool runPump = false;
    bool toggleLight = false;

    cloud.checkCommands(runPump, toggleLight);

        if (runPump) {
            // Force the pump on manually
            // You might need to add a public "forcePump()" method to your Plant class
            myPlant.manualPumpTrigger(); 
        }

        if (toggleLight) {
            // Force light toggle
            myPlant.manualLightToggle();
        }
}