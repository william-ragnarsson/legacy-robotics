int micVolume = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  micVolume = analogRead(0);
  Serial.println(micVolume);
  delay(5);
}
