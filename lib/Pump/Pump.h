#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

/**
 * @brief Wrapper class for a Relay-Controlled Water Pump.
 * Provides abstraction for turning a high-power pump ON/OFF safely
 * using a digital GPIO pin.
 */
class Pump {
  private:
    /**
     * @brief The GPIO pin connected to the Relay Input.
     */
    int _pin;

  public:
    /**
     * @brief Construct a new Pump object.
     * * @param pin The GPIO pin number (e.g., 12, 14).
     * Warning: Do not use Input-Only pins (34, 35, 36, 39) for pumps.
     */
    Pump(int pin);

    /**
     * @brief Configures the hardware pin as an OUTPUT.
     * Also sets the initial state to OFF (LOW) to prevent accidental
     * watering during system reboot.
     */
    void begin();

    /**
     * @brief Activates the pump.
     * Sends a HIGH signal to the relay.
     * Note: If using an "Active LOW" relay, this logic might be inverted.
     */
    void turnOn();

    /**
     * @brief Deactivates the pump.
     * Sends a LOW signal to the relay.
     */
    void turnOff();
};

#endif