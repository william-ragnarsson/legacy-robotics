unsigned char matrix[1280] = {0};

void setup() {
  Serial.begin(9600);

  matrix[4] = 56;
  matrix[5] = 3;

  matrix[1021] = 24;
}

void loop() {  
  Serial.println(matrix[4]);
}
