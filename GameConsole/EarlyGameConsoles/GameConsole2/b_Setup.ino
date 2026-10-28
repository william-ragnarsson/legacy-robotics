void setup(){
  Serial.begin(9600);

  pinMode(JSPS, INPUT);
  digitalWrite(JSPS, HIGH);

  TFTInit();

  DrawTextBox(20, 49, 120, 30, 21,  "START", WHITE, BLACK);
}
