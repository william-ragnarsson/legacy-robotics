int potPin = A2;
float val = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  val = analogRead(potPin);
  val = map(val, 0, 1023, -63, 0);
  if(val == -35){
    val = -36;
  }
  Serial.println(val);
}
