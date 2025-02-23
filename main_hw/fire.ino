//Code for smart home

//Serial communicating with ESP32 with serial and controlling RFID RC-522

//Import librarys need my this project
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <ESP32Servo.h>

//Wifi information
#define WIFI_SSID "Twf"
#define WIFI_PASSWORD "12345678"

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

//Servo
int  servoPinDOOR_LIVING = 33;
int  servoPinWINDOW_LIVING = 25;
int  servoPinDOOR_BEDROOM = 26;
int  servoPinWINDOW_BEDROOM = 27;
int  servoPinWINDOW_KITCHEN = 12;
int  servoPinDOOR_KITCHEN = 13;
int  servoPinDOOR_BATHROOM = 32;

//LED -> relays to control leds
#define LED_LIVING 19
#define LED_BEDROOM 21
#define LED_KITCHEN 34
#define LED_BATHROOM 35

//Fan -> pin relay
#define FAN_LIVING 5
#define FAN_BEDROOM 18
#define FAN_KITCHEN 22
#define FAN_BATHROOM 23

// #define SMOKE_RELAY x

//Define DHT11
DHT dht(DHTPIN, DHTTYPE);
//Define servo
Servo DOOR_LIVING;
Servo WINDOW_LIVING;
Servo DOOR_BEDROOM;
Servo WINDOW_BEDROOM;
Servo DOOR_KITCHEN;
Servo WINDOW_KITCHEN;
Servo DOOR_BATHROOM;

//Define object for Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

//Functions of sensors

//Global var
float humidity;
float temperature;
float smoke;
float lighting;
int fire;
float rain;

//DHT11 sensor function
void dht11Sensor(){
	humidity = dht.readHumidity();
	temperature = dht.readTemperature();
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
  Declearing information
  smoke = analogRead(mq2Pin);
  float smokeInfo = map(smoke, 0, 1023, 0, 100);
  //Displaying information on monitor serial
  Serial.print("Smoke: ");
  Serial.print(smokeInfo);
  Serial.println("%");
  //Seding information to Firebase
    if (Firebase.setFloat(fbdo, "/Smoke", smokeInfo)) {
     
     Serial.println("Sent information sucessful!");

    }
	
    if (smokeInfo > 50) {
            digitalWrite(SMOKE_RELAY, HIGH);  
            Serial.println("HAVE FIRE!!!");
        } else {
            digitalWrite(SMOKE_RELAY, LOW);
            Serial.println("SAFED");
        }
    } 
// //Rain sensor
void rainSensor(){
	
	lighting = analogRead(rainPin);
	
  float rainInfo = map(smoke, 0, 1023, 0, 100);
  
  //Displaying information on monitor serial
  
  Serial.print("Rain: ");
  
  Serial.print(rainInfo);
  
  Serial.println("%");
  
  //Seding information to Firebase
  
    if (Firebase.setFloat(fbdo, "/Rain", lightingInfo)) {
     
     Serial.println("Sent information sucessful!");

    }
    if (rainInfo > 50) {
    	//Servo using for handing clothes if rain > 50% and then take clothes in the house
    	//Code here area
	}
	else{
		
	}
	
}
//Fire sensor
void fireSensor(){
	fire = digitalRead(firePin);
	
  //Displaying information on monitor serial
  
  Serial.print("Fire status: ");
  
  Serial.print(fire);
  
  //Seding information to Firebase
  
    if (Firebase.setFloat(fbdo, "/Fire", fire)) {
     
     Serial.println("Sent information sucessful!");

    }
	
}

//LDR sensor
void ldrSensor(){
	
	lighting = analogRead(ldrPin);
	
  float lightingInfo = map(smoke, 0, 1023, 0, 100);
  
  //Displaying information on monitor serial
  
  Serial.print("Lighting: ");
  
  Serial.print(lightingInfo);
  
  Serial.println("%");
  
  //Seding information to Firebase
  
    if (Firebase.setFloat(fbdo, "/Light", lightingInfo)) {
     
     Serial.println("Sent information sucessful!");

    }
	
}


//FAN FOR ROOMS
//BEDROM
void firebaseRelayControlBEDROOM() {
  String path = "/Devices/bedroom/Fan";

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
            digitalWrite(FAN_BEDROOM, HIGH);  
            Serial.println("Fan on");
        } else {
            digitalWrite(FAN_BEDROOM, LOW);  
            Serial.println("Fan off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
    }
}
//KITCHEN
void firebaseRelayControlKITCHEN() {
  String path = "/Devices/kitchen/Fan";

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
            digitalWrite(FAN_KITCHEN, HIGH);  
            Serial.println("Fan on");
        } else {
            digitalWrite(FAN_KITCHEN, LOW);  
            Serial.println("Fan off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
    }
}
//BATHROOM
void firebaseRelayControlBATHROOM() {
  String path = "/Devices/bathroom/Fan";

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
            digitalWrite(FAN_BATHROOM, HIGH);  
            Serial.println("Fan on");
        } else {
            digitalWrite(FAN_BATHROOM, LOW);  
            Serial.println("Fan off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
    }
}
//LIVING ROOM
void firebaseRelayControlLIVING() {
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
            digitalWrite(FAN_LIVING, HIGH);  
            Serial.println("Fan on");
        } else {
            digitalWrite(FAN_LIVING, LOW);  
            Serial.println("Fan off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
    }
}
//FUNCTIONS FOR LED
//LIVING ROOM
void ledF_LIVING(){
	 String path = "/Devices/living/Light";

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("LED");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);

        // cheking status of firebase
        if (relayStatus == "ON") {
            digitalWrite(LED_LIVING, HIGH);  
            Serial.println("Bulb on");
        } else {
            digitalWrite(LED_LIVING, LOW);  
            Serial.println("Bulb off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
}

//BEDROOM
void ledF_BEDROOM(){
	 String path = "/Devices/bedroom/Light";

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("LED");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);

        // cheking status of firebase
        if (relayStatus == "ON") {
            digitalWrite(LED_BEDROOM, HIGH);  
            Serial.println("Bulb on");
        } else {
            digitalWrite(LED_BEDROOM, LOW);  
            Serial.println("Bulb off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
}

//KITCHEN
void ledF_KITCHEN(){
	 String path = "/Devices/kitchen/Light";

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("LED");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);

        // cheking status of firebase
        if (relayStatus == "ON") {
            digitalWrite(LED_KITCHEN, HIGH);  
            Serial.println("Bulb on");
        } else {
            digitalWrite(LED_KITCHEN, LOW);  
            Serial.println("Bulb off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
}
//BATHROOM
void ledF_BATHROOM(){
	 String path = "/Devices/bathroom/Light";

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("LED");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);

        // cheking status of firebase
        if (relayStatus == "ON") {
            digitalWrite(LED_BATHROOM, HIGH);  
            Serial.println("Bulb on");
        } else {
            digitalWrite(LED_BATHROOM, LOW);  
            Serial.println("Bulb off");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
}

//LIVING ROOM DOOR
//RFID + SERVOR
void DOORF_LIVING(){
	String path = "/Devices/living/Door";
	//Using arduino board to readed rfid tag and then send to esp32
	//esp32 will read if id of rfid tag == hex code of rfid that side and then enable this servo  

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Door ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
        
		//Code here area
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            DOOR_LIVING.write(0);
			delay(100); 
			DOOR_LIVING.write(90);
            Serial.println("DOOR OPEN");
        } else {
            DOOR_LIVING.write(180);
			delay(100);
			DOOR_LIVING.write(0);  
            Serial.println("DOOR CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}	
}
//BEDROOM DOOR
void DOORF_BEDROOM(){
	
	String path = "/Devices/bedroom/Door";  

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Door ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            DOOR_BEDROOM.write(0);
			delay(100); 
			DOOR_BEDROOM.write(90);
            Serial.println("DOOR OPEN");
        } else {
            DOOR_BEDROOM.write(180);
			delay(100);
			DOOR_BEDROOM.write(0);  
            Serial.println("DOOR CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}	
}
//BATHROOM DOOR
void DOORF_BATHROOM(){
	String path = "/Devices/bathroom/Door";	

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Door ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
        
		//Code here area
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            DOOR_BATHROOM.write(0);
			delay(100); 
			DOOR_BATHROOM.write(90);
            Serial.println("DOOR OPEN");
        } else {
            DOOR_BATHROOM.write(180);
			delay(100);
			DOOR_BATHROOM.write(0);  
            Serial.println("DOOR CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}	
}
//KITCHEN ROOM DOOR
void DOORF_KITCHEN(){
	String path = "/Devices/kitchen/Door";	

    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Door ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
        
		//Code here area
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            DOOR_KITCHEN.write(0);
			delay(100); 
			DOOR_KITCHEN.write(90);
            Serial.println("DOOR OPEN");
        } else {
            DOOR_KITCHEN.write(180);
			delay(100);
			DOOR_KITCHEN.write(0);  
            Serial.println("DOOR CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}	
}

//WINDOW FUNCTIONS
//LIVING
void WINDOWF_LIVING(){
	
	String path = "Devices/living/Window";
	
    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Window ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
        
		//Code here area
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            WINDOW_LIVING.write(0);
			delay(100); 
			WINDOW_LIVING.write(90);
            Serial.println("WINDOW OPEN");
        } else {
            WINDOW_LIVING.write(180);
			delay(100);
			WINDOW_LIVING.write(0);  
            Serial.println("WINDOW CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
	
}
//BEDROOM
void WINDOWF_BEDROOM(){
	
	String path = "Devices/bedroom/Window";
	
    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Window ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
        
		//Code here area
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            WINDOW_BEDROOM.write(0);
			delay(100); 
			WINDOW_BEDROOM.write(90);
            Serial.println("WINDOW OPEN");
        } else {
            WINDOW_BEDROOM.write(180);
			delay(100);
			WINDOW_BEDROOM.write(0);  
            Serial.println("WINDOW CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
	
}
	
//KITCHEN
void WINDOWF_KITCHEN(){
	
	String path = "Devices/kitchen/Window";
	
    if (Firebase.getString(fbdo, path)) {
        // give info from firebase side
        String relayStatus = fbdo.stringData();

        // Displaying infomation from serial monitor
        Serial.print("Window ");
        Serial.print(path);
        Serial.print(" Status: ");
        Serial.println(relayStatus);
        
		//Code here area
		
        // cheking status of firebase
        if (relayStatus == "ON") {
            WINDOW_KITCHEN.write(0);
			delay(100); 
			WINDOW_KITCHEN.write(90);
            Serial.println("WINDOW OPEN");
        } else {
            WINDOW_KITCHEN.write(180);
			delay(100);
			WINDOW_KITCHEN.write(0);  
            Serial.println("WINDOW CLOSED");
        }
    } else {
        Serial.print("Error");
        Serial.println(fbdo.errorReason());
	
	}
	
}

//Funtions for rooms

//LIVING ROOM
void livingRoom(){
	DOORF_LIVING();
	WINDOWF_LIVING();
	ledF_LIVING();
	firebaseRelayControlLIVING();
}
void bedRoom(){
	DOORF_BEDROOM();
	WINDOWF_BEDROOM();
	ledF_BEDROOM();
	firebaseRelayControlBEDROOM();
}
void bathRoom(){
	DOORF_BATHROOM();
	ledF_BATHROOM();
	firebaseRelayControlBATHROOM();	
}
void kitchenRoom(){
	DOORF_KITCHEN();
	WINDOWF_KITCHEN();
	ledF_KITCHEN();
	firebaseRelayControlKITCHEN();
}
void summarryInfo(){
	dht11Sensor();
	mq2Sensor();
	fireSensor();
  ldrSensor();
	rainSensor();	
}


void setup() {
    pinMode(FAN_LIVING, OUTPUT);
    pinMode(FAN_BEDROOM, OUTPUT);
    pinMode(FAN_KITCHEN, OUTPUT);
    pinMode(FAN_BATHROOM, OUTPUT);

    pinMode(LED_LIVING, OUTPUT);
    pinMode(LED_BEDROOM, OUTPUT);
    pinMode(LED_KITCHEN, OUTPUT);
    pinMode(LED_BATHROOM, OUTPUT);


    pinMode(mq2Pin, INPUT);
    pinMode(ldrPin, INPUT);
    pinMode(firePin, INPUT);
    pinMode(rainPin, INPUT);


    Serial.begin(9600);
    dht.begin();

    //Servo
    DOOR_LIVING.attach(servoPinDOOR_LIVING);
    WINDOW_LIVING.attach(servoPinWINDOW_LIVING);
    DOOR_BEDROOM.attach(servoPinDOOR_BEDROOM);
    WINDOW_BEDROOM.attach(servoPinWINDOW_BEDROOM);
    DOOR_BATHROOM.attach(servoPinDOOR_BATHROOM);
    DOOR_KITCHEN.attach(servoPinDOOR_KITCHEN);
    WINDOW_KITCHEN.attach(servoPinWINDOW_KITCHEN);


    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Đang kết nối");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      Serial.println(WiFi.status());
    }
    Serial.println("Wifi connected!");
    Serial.println(WiFi.status());

    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;


    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void loop() {
  
	livingRoom();
	bedRoom();
	bathRoom();
	kitchenRoom();
	summarryInfo();

}