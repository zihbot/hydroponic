void setup() { pinMode(D6, OUTPUT); }

void loop() {
  digitalWrite(D6, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);             // wait for a second
  digitalWrite(D6, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);             // wait for a second
}