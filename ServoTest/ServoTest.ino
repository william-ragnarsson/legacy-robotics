#include <Servo.h>;
Servo servo;

const int potPin = A0;
const int servoPin = 2;

void setup() {
  pinMode(potPin, INPUT);
  pinMode(servoPin, OUTPUT);

  servo.attach(servoPin);
  
  Serial.begin(9600);
}

void loop() {
  int potVal = analogRead(potPin);
  int servoVal = map(potVal, 0, 1023, 180, 0);

  servo.write(servoVal);
  
  Serial.println(servoVal);
}
