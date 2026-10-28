#include <Wire.h>
#include <NintendoExtensionCtrl.h>

Nunchuk nunChuk;

#define SCL1 5
#define SDA1 4

#define SCL2 0
#define SDA2 2

bool Z1, c1, Z2, c2;
int joyX1, joyY1, joyX2, joyY2;

void UpdateControllerValues(){
  Wire.begin(SDA1, SCL1);
  bool succes1 = nunChuk.update();
  if(!succes1){
    Serial.println("Lost bus #1 connection");
  }
  else{
    joyX1 = nunChuk.joyX();
    joyY1 = nunChuk.joyY();

    Z1 = nunChuk.buttonZ();
    c1 = nunChuk.buttonC();
  }



  Wire.begin(SDA2, SCL2);
  bool succes2 = nunChuk.update();
  if(!succes2){
    Serial.println("Lost bus #1 connection");
  }
  else{
    joyX2 = nunChuk.joyX();
    joyY2 = nunChuk.joyY();

    Z2 = nunChuk.buttonZ();
    c2 = nunChuk.buttonC();
  }
}

void ConnectController(){
  Wire.begin(SDA1, SCL1);
  while (!nunChuk.connect()) {
    Serial.println("Nunchuk on bus #1 not detected!");
    delay(1000);
  }
  Serial.println("Nunchuck on bus #1 connected!");

  Wire.begin(SDA2, SCL2);
  while (!nunChuk.connect()) {
    Serial.println("Nunchuk on bus #2 not detected!");
    delay(1000);
  }
  Serial.println("Nunchuck on bus #2 connected!");
}

void PrintValues(){
  Serial.print(joyX1); Serial.print(" ");
  Serial.print(joyY1); Serial.print(" ");
  Serial.print(Z1); Serial.print(" ");
  Serial.print(c1); Serial.print(" ");

  Serial.print("     ");

  Serial.print(joyX2); Serial.print(" ");
  Serial.print(joyY2); Serial.print(" ");
  Serial.print(Z2); Serial.print(" ");
  Serial.println(c2);
}

void setup() {
  Serial.begin(115200);

	ConnectController();
}

void loop() {
  UpdateControllerValues();

  PrintValues();
}