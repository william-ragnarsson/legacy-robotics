extern const uint16_t BLACK = 0x0000;
extern const uint16_t WHITE = 0xffff;

void setup() {
  Serial.begin(9600);
  
  TFTInit();
}

void loop() {
  DrawRect(50, 34, 53, 34, WHITE);

  
}
