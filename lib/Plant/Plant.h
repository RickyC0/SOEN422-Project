#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>
#include "DebugMacros.h"
#include "SystemConfig.h"

#include "Pump.h"
#include "MoistureSensor.h"
#include "LightSensor.h"

class Plant {
  private:
    // --- COMPONENTS ---
    Pump _pump;
    MoistureSensor _moisture;
    LightSensor _lightSensor;

    // --- PINS ---
    int _pinRed, _pinGreen, _pinBlue;

    // --- STATE VARIABLES ---
    enum WateringState { W_IDLE, W_PUMPING, W_COOLDOWN };
    WateringState _waterState;
    bool _lightingActive;

    // --- TIMERS (For tracking time) ---
    unsigned long _waterTimer; // Tracks pump duration/cooldown
    unsigned long _lightCheckTimer; // Tracks light checking interval
    unsigned long _moistureCheckTimer; // Tracks moisture checking interval

    // --- SETTINGS (Loaded from Config via Constructor) ---
    int _moistThreshold;
    int _lightThreshold;
    unsigned long _pumpDuration;
    unsigned long _pumpCooldown;
    unsigned long _moistCheckInterval;
    unsigned long _lightCheckInterval;
    
    // --- PEEKING VARIABLES ---
    bool _isPeekingLight; 
    unsigned long _peekLightTimer; 
    unsigned long _pauseLightInterval;

  public:
    /**
     * @brief Construct a new Plant object. 
     * Uses default arguments from SystemConfig.h unless overridden.
     * * @param pumpPin GPIO pin for the water pump relay
     * @param moistPin ADC pin for the soil moisture sensor
     * @param lightPin ADC pin for the photoresistor
     * @param red_led_Pin PWM pin for Red LED channel
     * @param green_led_Pin PWM pin for Green LED channel
     * @param blue_led_Pin PWM pin for Blue LED channel
     * @param dry_moist_sensor_val Raw ADC value for "0% Wet" (Air)
     * @param wet_moist_sensor_val Raw ADC value for "100% Wet" (Water)
     * @param moist_sensor_thresh Percentage (0-100) below which watering triggers
     * @param light_sensor_thresh Light Level (0-4) below which lights turn on
     * @param pump_for_ms Duration to run pump in milliseconds
     * @param pump_cooldown_ms Time to wait after watering before checking again
     * @param moist_check_interval_ms How often to read soil moisture
     * @param light_check_interval_ms How often to read ambient light
     * @param pause_light_interval_ms How long to turn off LED to peek at ambient light
     */
    Plant(int pumpPin = PUMP_PIN, int moistPin = MOISTURE_PIN, int lightPin = LIGHT_SENSOR_PIN, 
          int red_led_Pin = RGB_RED_PIN, int green_led_Pin = RGB_GREEN_PIN, int blue_led_Pin = RGB_BLUE_PIN,
          int dry_moist_sensor_val = MOISTURE_DRY, int wet_moist_sensor_val = MOISTURE_WET,
          int moist_sensor_thresh = MOISTURE_THRESHOLD_PERCENT, int light_sensor_thresh = LIGHT_THRESHOLD_LEVEL,
          unsigned long pump_for_ms = PUMP_DURATION_MS, unsigned long pump_cooldown_ms = WATERING_COOLDOWN_MS,
          unsigned long moist_check_interval_ms = MOISTURE_CHECK_INTERVAL_MS, 
          unsigned long light_check_interval_ms = LIGHT_CHECK_INTERVAL_MS, unsigned long pause_light_interval_ms = LIGHT_PAUSE_INTERVAL_MS);
    
    /**
     * @brief Initializes all hardware components.
     * Sets pin modes and calls begin() on sensors.
     */
    void begin();

    /**
     * @brief Main logic loop for the plant.
     * Should be called inside void loop(). Handles non-blocking updates for
     * watering and lighting state machines.
     */
    void update();

    /**
     * @brief Manages the Watering State Machine.
     * Transitions between IDLE, PUMPING, and COOLDOWN states based on timers and moisture levels.
     * * @param now Current millis() time
     */
    void manageWatering(unsigned long now);

    /**
     * @brief Manages the Lighting Logic.
     * Handles the "Active Peeking" algorithm to check ambient light without feedback loops.
     * * @param now Current millis() time
     */
    void manageLighting(unsigned long now);

    /**
     * @brief Helper to write values to the RGB LED pins.
     * * @param r Red Value (0-255)
     * @param g Green Value (0-255)
     * @param b Blue Value (0-255)
     */
    void manageRGB(int r, int g, int b);

    /**
     * @brief Manually triggers a watering cycle.
     * Called from the Web Dashboard command listener.
     */
    void manualPumpTrigger();

    /**
     * @brief Manually toggles the grow lights on/off.
     * Called from the Web Dashboard command listener.
     */
    void manualLightToggle();

    // --- GETTERS FOR WEBSITE ---
    // These allow the website/FirebaseManager to peek at private variables

    /**
     * @brief Get the current Soil Moisture Percentage.
     * * @return int (0-100)
     */
    int getMoisturePercent() { return _moisture.getPercentage(); }

    /**
     * @brief Get the current Light Status Level.
     * * @return int (0=Dark, 4=Very Bright)
     */
    int getLightStatus() { return _lightSensor.getLightStatus(); }

    /**
     * @brief Checks if the water pump is currently active.
     * * @return true if pumping
     * @return false if idle or cooldown
     */
    bool isPumpRunning() { return (_waterState == W_PUMPING); }

    /**
     * @brief Checks if the grow lights are currently on.
     * * @return true if lights are ON
     * @return false if lights are OFF
     */
    bool isLightOn() { return _lightingActive; }

    /**
     * @brief Get the human-readable name of this plant node.
     * * @return String (e.g., "Plant 1 (Living Room)")
     */
    String getPlantName() { return PLANT_NAME; }
};

#endif