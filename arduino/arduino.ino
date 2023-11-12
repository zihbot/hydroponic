void setup() {
  pinMode(A0, OUTPUT);
}

void loop() {
  digitalWrite(A0, HIGH);
  delay(500);
  analogWrite(A0, LOW);
  delay(500);
}
