char Mymessage[5] = "dtyjjtyrd"; //String data

  void setup() {
    Serial.begin(9600);
  }

  void loop() {
    Serial.write(Mymessage,5); //Write the serial data
    delay(1000);

  }
