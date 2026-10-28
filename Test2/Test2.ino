void setup() {
  Serial.begin(9600);

  pinMode(A0, INPUT);
  pinMode(5, INPUT);
}

void loop() {
  Serial.print(analogRead(A0));
  Serial.print(" ");
  Serial.println(digitalRead(5));
}