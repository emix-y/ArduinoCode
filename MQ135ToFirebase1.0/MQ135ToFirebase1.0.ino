#include <MQ135.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Firebase project credentials
#define FIREBASE_HOST "air-quality-39d9a-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "Bpnq2YO4H3iV5dX0XUiFv9jiOHGdQhkmH4uMOCCh"

// WiFi credentials
#define WIFI_SSID "HUAWEI-E5330-9920"
#define WIFI_PASSWORD "14et72by"

// MQ135 sensor pin
#define MQ_PIN 2

// MQ135 sensor calibration values
#define RL_VALUE 10.0
#define RO_CLEAN_AIR_FACTOR 3.8
#define CALIBRATION_SAMPLE_TIMES 50
#define CALIBRATION_SAMPLE_INTERVAL 5000

// MQ135 sensor reading values
#define READ_SAMPLE_INTERVAL 5000
#define READ_SAMPLE_TIMES 5

// Create MQ135 sensor object
MQ135 gasSensor = MQ135(MQ_PIN);

// Create FirebaseData object for Firebase communication
FirebaseData firebaseData;

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Perform sensor calibration
  float rs = 0;
  for (int i = 0; i < CALIBRATION_SAMPLE_TIMES; i++) {
    rs += gasSensor.getResistance();
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  float r0 = rs / CALIBRATION_SAMPLE_TIMES / RO_CLEAN_AIR_FACTOR;

  // Calculate air quality in parts per million (PPM)
  float ratio = r0 / RL_VALUE;
  float ppm = pow(10, ((log10(ratio) - 1.2553) / -0.5181));

  // Print air quality reading to serial monitor
  Serial.print("Air quality (PPM): ");
  Serial.println(ppm);

  // Upload air quality reading to Firebase
  if (Firebase.ready()) {
    // Generate Firebase database path for current reading
    String path = "/air_quality_data/" + Firebase.pushInt("/air_quality_data", ppm) + "/ppm";
    
    // Set JSON data to upload to Firebase
    firebaseData.setJSON(firebaseData, "{\"ppm\":" + String(ppm) + "}");

    // Update Firebase node with air quality data
    if (Firebase.updateNode(firebaseData, path))
      Serial.println("Data uploaded to Firebase");
    else
      Serial.println("Error uploading data to Firebase");
  }

  // Wait before taking next reading
  delay(READ_SAMPLE_INTERVAL);
}

