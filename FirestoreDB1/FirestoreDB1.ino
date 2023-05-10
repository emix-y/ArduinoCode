#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <Firestore.h>

// Set up Wi-Fi
const char* ssid = "HUAWEI-E5330-9920";
const char* password = "14et72by";

// Set up Firebase
#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDEQqMBiggGiWPWqPO8ADu_NXykXTGErjs"
FirebaseData firebaseData;

// Set up Firestore
#define FIRESTORE_HOST "firestoredb1-26415"
#define FIRESTORE_CERT ""
#define FIRESTORE_API_KEY "AIzaSyDEQqMBiggGiWPWqPO8ADu_NXykXTGErjs"
FirebaseFirestore firestore;

// Set up the DHT sensor
#define DHTPIN 14          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

void setup() {
  Serial.begin(9600);
  dht.begin(); // Initialize DHT sensor

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Initialized Firebase");

  // Initialize Firestore
  firestore.begin(FIRESTORE_HOST, FIRESTORE_API_KEY, FIRESTORE_CERT);
  Serial.println("Initialized Firestore");
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Print results to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);

  // Create a JSON object with the sensor data
  DynamicJsonDocument json(1024);
  json["temperature"] = temperature;
  json["humidity"] = humidity;

  // Send the sensor data to Firestore
  if (firestore.setDocument(firebaseData, "sensor-data", "reading", json.as<JsonObject>())) {
    Serial.println("Data written to Firestore");
  } else {
    Serial.println("Error writing data to Firestore");
  }

  delay(10000); // Wait 10 seconds before taking another reading
}

