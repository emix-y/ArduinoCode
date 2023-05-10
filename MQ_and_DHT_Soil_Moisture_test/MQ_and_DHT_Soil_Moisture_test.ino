#include <DHT.h>

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
  dht.begin(); // Initialize DHT sensor
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Read raw sensor value from MQ-135 sensor
  sensorValue = analogRead(MQPIN);
  
  // Adjust sensor value based on temperature and humidity
  correctedValue = sensorValue * (1.0 + (0.00075 * (temperature - 25.0))) * (1.0 + (0.0018 * (humidity - 50.0)));
  
  // Print results to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
  Serial.print(" %\tGas concentration: ");
  Serial.print(correctedValue);
  Serial.println(" ppm");
  
  // Read soil moisture sensor
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoistureValue);
  Serial.println();

  delay(1000); // Wait 1 second before taking another reading
}
