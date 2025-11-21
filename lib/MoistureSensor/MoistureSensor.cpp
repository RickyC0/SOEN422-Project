#include "MoistureSensor.h"

MoistureSensor::MoistureSensor(int pin, int dryVal, int wetVal) {
  _pin = pin;
  _dryVal = dryVal;
  _wetVal = wetVal;
}

// Setup function to be called once
void MoistureSensor::begin() {
  // ESP32 specific setting: allows reading up to ~3.3V
  analogSetAttenuation(ADC_11db); 
  pinMode(_pin, INPUT);
}

// The method to get the actual percentage
int MoistureSensor::getPercentage() {
  int rawValue = analogRead(_pin);
  
  // Map the raw value to percentage (0-100)
  // _dryVal to represent 0% (completely dry)
  // _wetVal to represent 100% (completely wet) depedning on calibration of the sensor
  // Note: Moisture sensors output lower values for wetter soil -> map loweer raw values to higher percentages
  int percent = map(rawValue, _dryVal, _wetVal, 0, 100);

  // Constrain keeps the result between 0 and 100, extra safety check
  return constrain(percent, 0, 100);
}

