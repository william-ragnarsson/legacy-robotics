class Bike{
  public:
    int posX = 7;
    int posY = 102;  
};

Bike bike1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  UpdateCounter(bike1);
  Serial.println(bike1.posX);
}

void UpdateCounter(Bike& test){
  test.posX += 1;
}
