int snakeDir[2] = {0, 0};
int snakePos[2] = {0, 0};

//TIMER
int period = 10;
unsigned long time_now = 0;

void setup() {
  Serial.begin(9600);

  TFTInit();
}

void loop() {
  UpdateSnakeDir();

  if(millis() > time_now + period){
    time_now = millis();
    snakePos[0] += snakeDir[0];
    snakePos[1] += snakeDir[1];

    DrawFillRect(snakePos[0], snakePos[1], 1, 1);
  }
}
