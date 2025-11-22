#include "LightSensor.h"

LightSensor::LightSensor(int pin) {
  _pin = pin;
}

void LightSensor::begin() {
  // ESP32 specific: allows reading up to ~3.3V
  analogSetAttenuation(ADC_11db);
  pinMode(_pin, INPUT);
}

int LightSensor::readRaw() {
  return analogRead(_pin);
}

int LightSensor::readAverage() {
  long sum = 0;
  
  // Serial.println("--- START SAMPLING ---");
  
  // Take 20 samples
  for (int i = 0; i < 20; i++) {
    int val = readRaw();
    
    // Serial.printf("Sample %d: %d\n", i, val); 
    
    sum += val; 
    delay(5); 
  }

  long avg = sum / 20;
  
  // Serial.printf("--- CALCULATION: %ld / 20 = %d ---\n", sum, avg);
  
  return avg;
}

int LightSensor::getLightStatus() {
  int value = readAverage();
  Serial.printf("Raw Light Value: %d\n", value);

  if (value < 40) {
    return 0;
  } else if (value < 800) {
    return 1;
  } else if (value < 2000) {
    return 2;
  } else if (value < 3200) {
    return 3;
  } else {
    return 4;
  }
}

String LightSensor::printLightStatus() {
  int status = getLightStatus();
  Serial.printf("Light Status Code: %d\n", status);
  switch (status) {
    case 0:
      return "Dark";
    case 1:
      return "Dim";
    case 2:
      return "Normal";
    case 3:
      return "Bright";
    case 4:
      return "Very Bright";
    default:
      return "Unknown";
  }
}