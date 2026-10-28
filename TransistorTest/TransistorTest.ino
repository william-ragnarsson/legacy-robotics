#include <Servo.h>

Servo servo1;
Servo servo2;
int servo1Pin = 7; 
int servo2Pin = 4;

int potPin = A5;

void setup() { 
  Serial.begin(9600);
  
  servo1.attach(servo1Pin); 
  servo2.attach(servo2Pin);
  servo1.write(0); 
  servo2.write(0);

  pinMode(potPin, INPUT);
}

void loop(){ 
  int potVal = analogRead(potPin);
  int potVal1 = map(potVal, 0, 1023, 180, 0);
  int potVal2 = 0;

  if (potVal1 > 90){
    potVal2 = map(potVal1, 90, 180, 180, 90);
  }
  else if(potVal1 <= 90){
    potVal2 = map(potVal1, 0, 90, 90, 0);
  }
  
  servo1.write(potVal1);
  servo2.write(potVal2);
  
  Serial.println(potVal);
}
