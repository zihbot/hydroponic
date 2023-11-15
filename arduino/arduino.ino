uint8_t PIN_IN_D0 = 5U;
uint8_t PIN_IN_A0 = A0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_IN_D0, INPUT);
  pinMode(PIN_IN_A0, INPUT);
  Serial.println("Hello World");
}

void loop() {
  int value_A0 = analogRead(PIN_IN_A0);
  int value_D0 = digitalRead(PIN_IN_D0);
  Serial.println("D = " + String(value_D0) + " A = " + String(value_A0));
  delay(1000);
}
