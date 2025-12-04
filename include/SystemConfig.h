#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/**
 * @brief Global System Configuration.
 * Acts as the "Master Switch" for selecting which physical plant node
 * this code is being compiled for. Also holds shared secrets and network credentials.
 */

// --- PLANT IDENTITY SELECTION ---
// Uncomment exactly one line below to select the hardware configuration
#include "plants/plant1_config.h"
// #include "plants/Plant2_Config.h"
// #include "plants/Plant3_Config.h"

// --- FIREBASE CREDENTIALS ---

/**
 * @brief Firebase Web API Key.
 * Found in Firebase Console -> Project Settings -> General -> Web API Key.
 * Acts as the ID card for the device to talk to Google services.
 */
#define FIREBASE_API_KEY "AIzaSyAoUInnpxkWkLnkLTI18t7BGXFgapRXAow"

/**
 * @brief Realtime Database URL.
 * The address of the JSON database.
 * @warning Do NOT include "https://" prefix for the ESP32 library.
 */
#define FIREBASE_DB_URL "esp32-plant-care-default-rtdb.firebaseio.com"

// --- STORAGE BUCKET CONFIGURATION (Crucial) ---

/**
 * @brief Storage Bucket ID for the WEBSITE (Frontend).
 * Used in index.html. Format usually ends in ".firebasestorage.app".
 */
#define STORAGE_BUCKET_ID_WEB "esp32-plant-care.firebasestorage.app"

// --- NETWORK CREDENTIALS ---

/**
 * @brief Wi-Fi SSID (Network Name).
 */
#define WIFI_SSID "Blackberry"

/**
 * @brief Wi-Fi Password.
 */
#define WIFI_PASS "ricky0100"

#endif // SYSTEM_CONFIG_H