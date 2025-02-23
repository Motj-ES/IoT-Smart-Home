#include<DHT11_by_me.h>
#include<Arduino.h>
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
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// Firebase initialization
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200); // Start the serial monitor
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to Wi-Fi
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { // Wait for Wi-Fi connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connected!");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true); // Auto-reconnect to Wi-Fi if disconnected

  // Initialize DHT11 sensor
  dht.begin();
}

void loop() {
  // Read temperature and humidity from the DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) { // Check if the readings are valid
    Serial.println("Error reading from DHT11!");
    return;
  }

  // Print the data to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C, ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send temperature data to Firebase
  if (Firebase.setFloat(firebaseData, "/sensor/temperature", temperature)) {
    Serial.println("Temperature updated successfully!");
  } else {
    Serial.println("Failed to update temperature: " + firebaseData.errorReason());
  }

  // Send humidity data to Firebase
  if (Firebase.setFloat(firebaseData, "/sensor/humidity", humidity)) {
    Serial.println("Humidity updated successfully!");
  } else {
    Serial.println("Failed to update humidity: " + firebaseData.errorReason());
  }

  // Wait 10 seconds before updating again
  delay(10000);
}


