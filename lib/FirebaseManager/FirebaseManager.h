#ifndef FIREBASE_MANAGER_H
#define FIREBASE_MANAGER_H

#include <Arduino.h>
#include <Firebase_ESP_Client.h>

/**
 * @brief Handles all Cloud Connectivity for the IoT Plant System.
 * * Responsibilities:
 * 1. Managing Wi-Fi and Firebase Authentication.
 * 2. Pushing sensor telemetry (JSON) to the Realtime Database.
 * 3. Listening for remote commands (Pump/Light) from the Web Dashboard.
 * * Note: Uses dynamic memory allocation (new/pointers) to prevent
 * stack overflow crashes during ESP32 boot.
 */
class FirebaseManager {
  private:
    /**
     * @brief The primary Firebase Data Object.
     * Handles the actual HTTP/WiFi connection and holds response data.
     * Allocated on Heap to save Stack memory.
     */
    FirebaseData* _fbdo;

    /**
     * @brief Authentication Credentials.
     * Stores the User UID and Token after sign-in (Anonymous or Email).
     */
    FirebaseAuth* _auth;

    /**
     * @brief Configuration Object.
     * Stores API Key, Database URL, and other settings.
     */
    FirebaseConfig* _config;
    
    /**
     * @brief The root database path for this specific device.
     * Example: "/plants/plant1"
     */
    String _plantPath;

    /**
     * @brief Connection Status Flag.
     * True only after Wi-Fi is connected and Firebase Token is generated.
     */
    bool _isReady;

  public:
    /**
     * @brief Construct a new Firebase Manager object.
     * * @param plantId The unique name of this node (e.g., "plant1").
     * Used to create the database path: /plants/{plantId}
     */
    FirebaseManager(String plantId);

    /**
     * @brief Initialize the Cloud Connection.
     * 1. Allocates memory for Firebase objects.
     * 2. Connects to Wi-Fi.
     * 3. Authenticates with Firebase (Anonymous mode).
     * 4. Waits for valid Token generation.
     */
    void begin();

    /**
     * @brief Uploads a snapshot of the current system state to the Cloud.
     * * @param moisture Soil moisture percentage (0-100).
     * @param lightStatus Light level code (0=Dark, 4=Very Bright).
     * @param pumpOn True if pump is currently running.
     * @param lightOn True if grow light is currently active.
     */
    void sendData(int moisture, int lightStatus, bool pumpOn, bool lightOn);

    /**
     * @brief Checks the database for remote commands from the website.
     * * Reads the "commands/pump" and "commands/lightToggle" paths.
     * If true, it updates the referenced variables and resets the database value to false.
     * * @param pumpCmd [OUT] Set to true if website requested watering.
     * @param lightCmd [OUT] Set to true if website requested light toggle.
     */
    void checkCommands(bool &pumpCmd, bool &lightCmd);
};

#endif