#include <Arduino.h>

// Project Pins, Passwords, etc.
#include "Config.h"

#include "MoistureSensor.h"
#include "Pump.h"

// Create objects for Moisture Sensor and Pump
MoistureSensor moistureSensor(MOISTURE_SENSOR_1_PIN, 4095, 0);
Pump pump(PUMP_1_RELAY_PIN);

void setup() {
  Serial.begin(115200);
  
  // Initialize Moisture Sensor and Pump
  moistureSensor.begin();
  pump.begin();
}

void loop() {
  // Read moisture percentage
  int moisturePercent = moistureSensor.getPercentage();
  
  // Print moisture level to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  // Simple logic: If moisture is below 30%, turn on the pump for 5 seconds
  if (moisturePercent < 30) {
    Serial.println("Moisture low! Turning on pump...");
    pump.turnOn();
    delay(5000); // Pump runs for 5 seconds
    pump.turnOff();
    Serial.println("Pump turned off.");
  } else {
    Serial.println("Moisture level adequate. Pump remains off.");
  }
  
  // Wait for a 10 seconds before next reading
  delay(10000);
}







