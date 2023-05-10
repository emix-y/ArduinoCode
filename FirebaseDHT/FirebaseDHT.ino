#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Set up Wi-Fi
const char* ssid = "HUAWEI-E5330-9920";
const char* password = "14et72by";

// Set up Firebase
#define FIREBASE_HOST "temp-and-humidity-ba37a-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "LlxVOoB502Cr4vAwBvIuZyYs09RRPSkLuDlpV3YT"
FirebaseData firebaseData;

// Set up the DHT sensor
#define DHTPIN 14          // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11     // DHT sensor type
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// Define the Data class
class DATA {
  public:
    float value;
    virtual const char* getFirebasePath() = 0;
};

// Define the Temperature class
class Temperature : public DATA {
  public:
    Temperature(float temperature) {
      value = temperature;
    }
    const char* getFirebasePath() {
      return "/temperature";
    }
};

// Define the Humidity class
class Humidity : public DATA {
  public:
    Humidity(float humidity) {
      value = humidity;
    }
    const char* getFirebasePath() {
      return "/humidity";
    }
};

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
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature= dht.readTemperature();
  float humidity = dht.readHumidity();

  // Create Temperature and Humidity objects
  Humidity humidityData = Humidity(humidity);
  Temperature temperatureData = Temperature(temperature);
 
  // Write sensor data to Firebase
  Firebase.pushFloat(firebaseData, "/DATA/TEMPERATURE", temperatureData.value);
  Firebase.pushFloat(firebaseData, "/DATA/HUMIDITY", humidityData.value);

  // Print results to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);

  delay(10000); // Wait 10 second before taking another reading
}
