//Import librarys
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <SPI.h>
#include <ESP32Servo.h>
// #include <MFRC522.h>

//Wifi information
#define WIFI_SSID "Kingshouse-Lau2-A1"
#define WIFI_PASSWORD "kingshouse2023"

//Declaring Firebase informations
#define FIREBASE_HOST "smarthome-4d2d1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "MIz0RA7GJHH2OGturZvjFa0Uv1BtqJWEKGpHXM6d"

//Define pins out for sensors

// DHT11 sensor
#define DHTPIN 15
#define DHTTYPE DHT11
//MQ2 sensor
#define mq2Pin 2
//LDR sensor
#define ldrPin 4
//Fire sensor
#define firePin 16
//Rain sensor
#define rainPin 17
//HC-SR04
#define hcsr04_echoPin 5
#define hcsr04_trigPin 18
//RFID
#define sdaPin 19
#define sckPin 21
#define mosiPin 34
#define misoPin 35
#define rstPin 32
//Servo
#define servoPin1 33
#define servoPin2 25
#define servoPin3 26
//Relay
#define relayWhistle 27
#define relayPin2 14
#define relayPin3 12
#define relayPin4 13

//Define DHT11
DHT dht(DHTPIN, DHTTYPE);
Servo servo1;
Servo servo2;
Servo servo3;
// MFRC522 rfid(sdaPin, rstPin);

//Define object for Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//Functions of sensors

//DHT11 sensor function
void dht11Sensor(){
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
    // Checking data read if one in informations none is print the string in downside
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Displaying informations on monitor serial
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");

    //Sending data to Firebase
    //Temperature
    if (Firebase.setFloat(fbdo, "/Temperature", temperature)) {
     
     Serial.println("Sent information sucessful!");

    } 
    else {

      Serial.println("Sent Error: " + fbdo.errorReason());

    }

    //Humidity
    if (Firebase.setFloat(fbdo, "/Humidity", humidity)) {
     
     Serial.println("Sent information sucessful!");

    } 
    else {

      Serial.println("Sent Error: " + fbdo.errorReason());

    }

  delay(500); 

}

//MQ2 Sensor
void mq2Sensor(){
  //Declearing information
  float smoke = analogRead(mq2Pin);
  float smokeInfo = map(smoke, 0, 1023, 0, 100);
  //Displaying information on monitor serial
  Serial.print("Smoke: ");
  Serial.print(smokeInfo);
  Serial.println("%");
  //Seding information to Firebase
      if (Firebase.setFloat(fbdo, "/Smoke", smokeInfo)) {
     
     Serial.println("Sent information sucessful!");

    } 
    else {

      Serial.println("Sent Error: " + fbdo.errorReason());

    }

  delay(500); 

}

//LDR Sensor
void ldrSensor(){
  //Declearing information
  float lighting = analogRead(ldrPin);
  float lightingInfo = map(lighting, 0, 4095, 0, 100);
  //Displaying information on monitor serial
  Serial.print("Lighting: ");
  Serial.print(lightingInfo);
  Serial.println("%");
  //Seding information to Firebase
      if (Firebase.setFloat(fbdo, "/Light", lightingInfo)) {
     
     Serial.println("Sent information sucessful!");

    } 
    else {

      Serial.println("Sent Error: " + fbdo.errorReason());

    }

  delay(500); 

}

//Fire Sensor
void fireSensor(){
  //Declearing information
  int fire = digitalRead(firePin);
  //Displaying information on monitor serial
  Serial.print("Fire: ");
  Serial.print(fire);
  Serial.println();
  //Seding information to Firebase
    if (Firebase.setFloat(fbdo, "/Fire", fire)) {
     
     Serial.println("Sent information sucessful!");

    } 
    else {

      Serial.println("Sent Error: " + fbdo.errorReason());

    }
    //Control relay + whistle
    //if has fire then whistle ring
    if(fire == 1){
      digitalWrite(relayWhistle, 1);
      delay(10000);
    }
    //else no ring
    else digitalWrite(relayWhistle, 0);

  delay(500); 

}

//Rain sensor
void rainSensor(){
  //Declearing information
  float rain = analogRead(rainPin);
  float rainInfo = map(rain, 0, 1023, 0, 100);
  String rainSend;
  //Control hanging-clothes
  if(rainInfo > 50){
    rainSend = "Rain";
    servo1.write(180);
  }
  else {
  servo1.write(0);
  rainSend = "No Rain";}
  //Displaying information on monitor serial
  Serial.print("Rain: ");
  Serial.print(rainSend);
  Serial.println();
  //Seding information to Firebase
      if (Firebase.setString(fbdo, "/Rain", rainSend)) {
     
     Serial.println("Sent information sucessful!");

    } 
    else {

      Serial.println("Sent Error: " + fbdo.errorReason());

    }

  delay(500); 

}
//HC-SR04
//function measure distance
long getDistance() {
  digitalWrite(hcsr04_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(hcsr04_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(hcsr04_trigPin, LOW);

  // measuring echo timmer give signal
  long duration = pulseIn(hcsr04_echoPin, HIGH);

  // (sound speed = 0.034 cm/us)
  long distance = duration * 0.034 / 2;

  return distance;
}
void hcsr04Sensor(){
    // measure distance from hcsr04
  long distance = getDistance();
  Serial.print("Distance measured: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Checking distance
  if (distance < 10) {
    servo2.write(180);  
    delay(5000);         
    servo2.write(0);    
  }

  delay(500); 
}

// //RFID
// byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF};

// // function for open door

// void openDoor() {
//   servo3.write(90);  
//   delay(5000);       
//   servo3.write(0);  
// }

// // Functions cheking uid of tag
// bool checkUID(byte *uid) {
//   for (byte i = 0; i < 4; i++) {
//     if (uid[i] != authorizedUID[i]) {
//       return false;
//     }
//   }
//   return true;
// }

// void rfidSensor(){
  
//   if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
//     delay(500);
//     return;
//   }

//   if (checkUID(rfid.uid.uidByte)) {
//     Serial.println("Th? h?p l?! M? c?a...");
//     openDoor(); 
//   } else {
//     Serial.println("Th? không h?p l?!");
//   }

//   rfid.PICC_HaltA();
//   delay(500);  

// }

//Relay

void firebaseRelayControl() {
  String path = "/Devices/living/Fan";

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Fan");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);

        // cheking status of firebase
        if (relayStatus == "ON") {
            digitalWrite(relayPin2, HIGH);  // Kích ho?t relay (b?t)
            Serial.println("Fan on");
        } else {
            digitalWrite(relayPin2, LOW);   // T?t relay
            Serial.println("Fan off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
    }
}



void setup() {
  pinMode(mq2Pin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(firePin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(hcsr04_trigPin, OUTPUT);
  pinMode(hcsr04_echoPin, INPUT);
  pinMode(relayWhistle, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);

  Serial.begin(115200);
  dht.begin();
  // SPI.begin();
  // rfid.PCD_Init();

  digitalWrite(relayWhistle, 0);
  digitalWrite(relayPin2, 0);
  digitalWrite(relayPin3, 0);
  digitalWrite(relayPin4, 0);

  //Servo
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Ðang k?t n?i Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi connected!");

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;


  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  
  dht11Sensor();
  mq2Sensor();
  ldrSensor();
  fireSensor();
  rainSensor();
  hcsr04Sensor();
  firebaseRelayControl();

}

