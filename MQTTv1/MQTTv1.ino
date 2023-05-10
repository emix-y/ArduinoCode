#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "HUAWEI-E5330-9920";
const char* password = "14et72by";
const char* mqtt_server = "r68ddea1.ala.us-east-1.emqxsl.com";
const int mqtt_port = 8883;
const char* mqtt_user = "emqx";
const char* mqtt_password = "emqx1234";
const char* mqtt_client_id = "emqx";
const char* mqtt_topic = "esp32/test";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  mqttClient.setServer(mqtt_server, mqtt_port);
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

void loop() {
  // Read sensor data and publish to MQTT topic
  float sensorData = analogRead(4);
  mqttClient.publish(mqtt_topic, String(sensorData).c_str());
  delay(1000);
}
