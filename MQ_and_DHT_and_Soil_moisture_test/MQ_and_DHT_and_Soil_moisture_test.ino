#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MQ135.h>

#define DHTPIN 14
#define DHTTYPE DHT11 
DHT_Unified dht(DHTPIN, DHTTYPE);

const int soilMoisturePin = A0;
MQ135 gasSensor = MQ135(A1);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT22 sensor
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" °C");
  } else {
    Serial.println("Error reading temperature!");
  }
  
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println(" %");
  } else {
    Serial.println("Error reading humidity!");
  }
  
  // Read soil moisture sensor
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoistureValue);
  Serial.println();
  
  // Read air quality sensor
  float airQualityValue = gasSensor.getPPM();
  Serial.print("Air Quality: ");
  Serial.print(airQualityValue);
  Serial.println(" ppm");
  
  delay(2000);
}
