const int ledPin = 2;
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}
void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON");
  delay(1200);
  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF");
  delay(1200);
}
