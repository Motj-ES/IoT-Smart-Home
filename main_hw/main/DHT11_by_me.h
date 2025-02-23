#ifndef DHT11_BY_ME_H
#define DHT11_BY_ME_H

#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include <FirebaseESP32.h>

// Wi-Fi credentials
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// Firebase information
#define FIREBASE_HOST "your_firebase_url"  // Firebase URL
#define FIREBASE_AUTH "your_firebase_auth" // Firebase secret key

// DHT11 sensor configuration
#define DHTPIN 4    // Pin where DHT11 is connected to ESP32
#define DHTTYPE DHT11 // DHT11 sensor type

// Declare DHT and FirebaseData objects
extern DHT dht;
extern FirebaseData firebaseData;

// Function prototypes
void setup();
void loop();

#endif // DHT11_BY_ME_H

