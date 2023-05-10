const int mqPin = 2;   // Replace XX with the digital pin number connected to the DO pin of the MQ-135 sensor

void setup() {
  Serial.begin(9600);
  pinMode(mqPin, INPUT);
}

void loop() {
  int mqVal = digitalRead(mqPin);
  Serial.print("MQ-135 value: ");
  Serial.println(mqVal);
  delay(1000);
}
