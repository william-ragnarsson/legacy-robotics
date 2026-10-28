int snakeBody[4][2];

snakeBody[0] = {9, 9};

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(snakeBody[1][0]);
}
