#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <Arduino.h>

/**
 * @brief Wrapper class for a Photoresistor (LDR) Sensor.
 * Handles analog reading, noise smoothing (averaging), and
 * mapping raw voltage to human-readable light levels.
 */
class LightSensor {
  private:
    /**
     * @brief The GPIO pin connected to the sensor's Analog Output.
     */
    int _pin;

  public:
    /**
     * @brief Construct a new Light Sensor object.
     * * @param pin The ADC pin (e.g., 33, 34, 35) connected to the sensor.
     * Note: Use ADC1 pins to avoid Wi-Fi conflicts.
     */
    LightSensor(int pin);

    /**
     * @brief Configures the hardware pin.
     * Sets the pin to INPUT mode and configures ADC attenuation 
     * to read the full 3.3V range.
     */
    void begin();

    /**
     * @brief Reads a single instantaneous value from the sensor.
     * * @return int Raw ADC value (0 to 4095).
     */
    int readRaw();

    /**
     * @brief Gets a simplified status code for the current light level.
     * Uses readAverage() internally for stability.
     * * @return int Status Code:
     * 0 = Dark
     * 1 = Dim
     * 2 = Normal
     * 3 = Bright
     * 4 = Very Bright
     */
    int getLightStatus();

    /**
     * @brief Gets a text description of the light level.
     * Useful for printing to Serial or displaying on an OLED screen.
     * * @return String e.g. "Dark", "Normal", "Very Bright"
     */
    String printLightStatus();

    /**
     * @brief Takes multiple samples to filter out electrical noise.
     * * @warning BLOCKING FUNCTION: This takes approx 100ms to complete 
     * (20 samples * 5ms delay). Do not call this too frequently 
     * inside a high-speed loop.
     * * @return int The averaged raw value.
     */
    int readAverage();
};

#endif