#include <FirebaseESP32.h>
#include <WiFi.h>
#include <DHT.h>

#define FIREBASE_HOST "https://arduino-data-bafb4-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "6CFcDV7FLdEb2nrrv7HAyteMkRGX6fZVluQUdfwa"
#define WIFI_SSID "HUAWEI-E5330-9920"
#define WIFI_PASSWORD "14et72by"

FirebaseData firebaseData;

// Set up the DHT sensor
#define DHTPIN 14          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// Set up the MQ-135 sensor
#define MQPIN 2         // Analog pin connected to the MQ-135 sensor
float sensorValue;      // Raw sensor value
float correctedValue;   // Sensor value adjusted for temperature and humidity

const int soilMoisturePin = 4;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Wait until connection is established
  while (!Firebase.ready()) {
    delay(100);
  }
}

void loop() {
  // Read sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float mqValue = correctedValue;
  int soilMoistureValue = analogRead(soilMoisturePin);

  // Build Firebase data object
  FirebaseJson json;
  json.set("temperature", temperature);
  json.set("humidity", humidity);
  json.set("mqValue", mqValue);
  json.set("soilMoistureValue", soilMoistureValue);

  // Send data to Firebase
  if (Firebase.pushJSON(firebaseData, "/sensorData", json)) {
    Serial.println("Data sent to Firebase!");
  } else {
    Serial.println("Error sending data to Firebase.");
    Serial.println(firebaseData.errorReason());
  }

  delay(1000); // Wait 1 second before taking another reading
}
