int buttonState = 0;
void setup() {
 pinMode(3, INPUT);
 Serial.begin(9600);
}
void loop(){
  buttonState = digitalRead(3);
  Serial.println(buttonState);
}
