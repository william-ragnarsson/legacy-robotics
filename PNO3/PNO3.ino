struct FlexSensor {
    int pin;
    int max;
    int min;
    int margin;
    
    FlexSensor(int p) : pin(p), max(850), min(100), margin(100) {
        pinMode(pin, INPUT);
    }
    int readValue() {
      return analogRead(pin);
    }
};

struct Valve {
    int pin;
    bool switchedOn;

    Valve(int p) : pin(p), switchedOn(false) {
        pinMode(pin, OUTPUT);
    }
    void on() {
      digitalWrite(pin, HIGH);
      switchedOn = true;
    }
    void off() {
      digitalWrite(pin, LOW);
      switchedOn = false;
    }
};

FlexSensor flexSensor(A0);

Valve hardZachtHard(12);
Valve hardZachtZacht(13);

Valve remmenGrijpen(4);
Valve remmenKnijpen(8);
Valve luchtzakkenGrijpen(2); 
Valve luchtzakkenKnijpen(0);

bool largeObject = true;
bool hardObject = true;
bool grijpen = true;
bool showFlexSensor = false;

void setup() {
  Serial.begin(9600);

  Init();
}

void loop() {

  if(Serial.available() > 0){
    String input = Serial.readStringUntil('\n');
    input.trim();

    if(input == "flex on"){
      showFlexSensor = true;
    } else if(input == "flex off"){
      showFlexSensor = false;
    } else if (input == "groot") {
      largeObject = true;
    } else if (input == "klein") {
      largeObject = false;
    } else if (input == "grijp") {
      grijpen = true;
    } else if (input == "knijp") {
      grijpen = false;
    } else if (input == "hard") {
      hardObject = true;
    } else if (input == "zacht") {
      hardObject = false;
    }
  }

  ToggleHardZacht();

  if(grijpen){
    Grijpen();
  } else{
    Knijpen();
  }

delay(10);
}

void Knijpen(){
  int flexSensorValue = flexSensor.readValue();

  if(showFlexSensor){
    Serial.println(flexSensorValue);
  }

  if(flexSensorValue < flexSensor.max){
    RemmenOn();
  } else{
    RemmenOff();
  }
}

void Grijpen(){
  int flexSensorValue = flexSensor.readValue();
  if(showFlexSensor){
    Serial.println(flexSensorValue);
  }
  
  if (largeObject && flexSensorValue < flexSensor.max){
    RemmenOn();    
    Serial.println("Large Object");
  } else if (flexSensorValue >  flexSensor.max + flexSensor.margin){
    RemmenOff();
    Serial.println("No Object");
  }

  if(!largeObject && flexSensorValue < flexSensor.min){
    RemmenOn();
    
  } else if (flexSensorValue > flexSensor.min + flexSensor.margin){
    RemmenOff();
  }
}

void RemmenOn(){

  if(grijpen){
    remmenGrijpen.on();
    remmenKnijpen.on();

    luchtzakkenGrijpen.on();
    luchtzakkenGrijpen.on();
  } else{
    remmenKnijpen.off();
    remmenKnijpen.on();

    luchtzakkenKnijpen.on();
    luchtzakkenGrijpen.off();
  }
}

void RemmenOff(){
  remmenGrijpen.off();
  remmenKnijpen.off();

  luchtzakkenGrijpen.off();
  luchtzakkenKnijpen.off();
}

void ToggleHardZacht(){
  if(hardObject){
    hardZachtZacht.off();
    hardZachtHard.on();
  } else{
    hardZachtHard.off();
    hardZachtZacht.on();
  }
}

void Init(){
  remmenGrijpen.off();
  remmenKnijpen.off();

  hardZachtHard.off();
  hardZachtZacht.off();

  luchtzakkenGrijpen.off();
  luchtzakkenKnijpen.off();
}