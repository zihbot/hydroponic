void setup() {
  pinMode(A0, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello World");
  digitalWrite(A0, HIGH);
  delay(500);
  analogWrite(A0, LOW);
  delay(500);
}
