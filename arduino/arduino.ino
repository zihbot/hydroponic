uint8_t PIN_IN_D0 = 5U;
uint8_t PIN_IN_A0 = A0;

const uint8_t PC1 = 13;
const uint8_t PC2 = 3;
const uint8_t PC3 = 4;
const uint8_t PC4 = 10;
const uint8_t PC5 = 6;
const uint8_t PC6 = 11;
const uint8_t PC7 = 15;
const uint8_t PC8 = 17;
const uint8_t PR1 = 9;
const uint8_t PR2 = 14;
const uint8_t PR3 = 8;
const uint8_t PR4 = 12;
const uint8_t PR5 = 1;
const uint8_t PR6 = 7;
const uint8_t PR7 = 2;
const uint8_t PR8 = 5;

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
