class Bike{
  public:
    int posX = 7;
    int posY = 102;  
};

Bike bike1;
Bike bike2;

Bike bikeArr[2] = {bike1, bike2};

void PrintSomething(Bike test){
  Serial.println(test.posX);
}

void setup() {
  Serial.begin(9600);

  bikeArr[0].posX = 364;
  PrintSomething(bike2);
}

void loop() {
  
}
