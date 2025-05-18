#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Wi-Fi Credentials
#define WIFI_SSID "WIFI SSID"
#define WIFI_PASSWORD "Password"

// Firebase Configuration
#define API_KEY "Firebase API Key"
#define DATABASE_URL "Firebase URL"

// Firebase objects
FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;

// Motor Pins for L298N
const int motorPin1 = 19;
const int motorPin2 = 18;

// TCS3200 Color Sensor Pins
#define S0 32
#define S1 33
#define S2 25
#define S3 26
#define sensorOut 35

// Soil Moisture Sensor Pin
const int soilMoisturePin = 34;  

// Function to calculate soil wet percentage
int getSoilWetPercentage(int value) {
    return map(value, 0, 4095, 100, 0);
}

void setup() {
    Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWi-Fi connected");
    Serial.println("IP Address: " + WiFi.localIP().toString());

    // Firebase Configuration
    firebaseConfig.api_key = API_KEY;
    firebaseConfig.database_url = DATABASE_URL;

    firebaseAuth.user.email = "";
    firebaseAuth.user.password = "";

    Firebase.begin(&firebaseConfig, &firebaseAuth);
    Firebase.reconnectWiFi(true);

    if (Firebase.signUp(&firebaseConfig, &firebaseAuth, "", "")) {
        Serial.println("Firebase Anonymous Authentication Successful!");
    } else {
        Serial.println("Firebase Auth Error: " + String(firebaseConfig.signer.signupError.message.c_str()));
    }

    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);

    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Set frequency scaling to 100%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);
}

// Helper function to map float values
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int getColorValue(int s2State, int s3State) {
    digitalWrite(S2, s2State);
    digitalWrite(S3, s3State);
    delay(100);
    return pulseIn(sensorOut, LOW);
}

void calculateAndSendNPK(int red, int green, int blue) {
    int maxColor = max(red, max(green, blue));
    if (maxColor == 0) return; // Avoid division by zero

    float R_norm = (red * 1.0) / maxColor;
    float G_norm = (green * 1.0) / maxColor;
    float B_norm = (blue * 1.0) / maxColor;

    int nitrogen = (int)((G_norm * 100) - 10);
    int phosphorus = (int)((R_norm * 100) - 5);
    int potassium = (int)((B_norm * 100) - 8);

    nitrogen = max(0, nitrogen);
    phosphorus = max(0, phosphorus);
    potassium = max(0, potassium);

    // Map nitrogen to 0–85 and potassium to 0–75
    nitrogen = (int)mapFloat(nitrogen, 0.0, 100.0, 0.0, 95.0);
    potassium = (int)mapFloat(potassium, 0.0, 100.0, 0.0, 80.0);

    Serial.printf("N: %d, P: %d, K: %d\n", nitrogen, phosphorus, potassium);

    // Send to Firebase as integers
    Firebase.RTDB.setInt(&firebaseData, "/soil/N", nitrogen);
    Firebase.RTDB.setInt(&firebaseData, "/soil/P", phosphorus);
    Firebase.RTDB.setInt(&firebaseData, "/soil/K", potassium);
}


void loop() {
    // Soil moisture sensor value
    int sensorValue = analogRead(soilMoisturePin);
    int wetPercentage = getSoilWetPercentage(sensorValue);
    Firebase.RTDB.setInt(&firebaseData, "/soil_moisture/wet_percentage", wetPercentage);

    // Read RGB values
    int red = getColorValue(LOW, LOW);
    int green = getColorValue(HIGH, HIGH);
    int blue = getColorValue(LOW, HIGH);

    calculateAndSendNPK(red, green, blue);

    // Motor Control
    String motorState;
    if (Firebase.RTDB.getString(&firebaseData, "/motor/state")) {
        motorState = firebaseData.stringData();
        Serial.print("Motor state from Firebase: ");
        Serial.println(motorState);

        if (motorState == "on") {
            digitalWrite(motorPin1, HIGH);
            digitalWrite(motorPin2, LOW);
            Serial.println("Motor ON");
        } else if (motorState == "off") {
            digitalWrite(motorPin1, LOW);
            digitalWrite(motorPin2, LOW);
            Serial.println("Motor OFF");
        }
    } else {
        Serial.println("Firebase Error: " + firebaseData.errorReason());
    }

    delay(1000); // Delay before next reading
}
