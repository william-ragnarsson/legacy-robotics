int potPin = A5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(potPin));
  delay(20);
}
