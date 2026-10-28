//ESP NOW
#include <ESP8266WiFi.h>
#include <espnow.h>

//NINTENDO CONTROLLER
#include <NintendoExtensionCtrl.h>

//ESP NOW
uint8_t broadcastAddress[] = { 0xB8, 0xD6, 0x1A, 0x44, 0x3B, 0x4C };

//NINTENDO CONTROLS
Nunchuk nunChuk;

struct struct_message {
  bool Z, c;
  int joyX, joyY;
};
struct_message structNunChuk;

void UpdateAnalogStick(){
  structNunChuk.joyX = map(nunChuk.joyX(), 35, 225, 0, 100);
  if(structNunChuk.joyX < 0) structNunChuk.joyX = 0;
  else if(structNunChuk.joyX > 100) structNunChuk.joyX = 100;

  structNunChuk.joyY = map(nunChuk.joyY(), 30, 220, 0, 100);
  if(structNunChuk.joyY < 0) structNunChuk.joyY = 0;
  else if(structNunChuk.joyY > 100) structNunChuk.joyY = 100;
}

void UpdateButtons(){
  structNunChuk.Z = nunChuk.buttonZ();
  structNunChuk.c = nunChuk.buttonC();
}


void setup() {
  //INIT SERIAL
  Serial.begin(115200);

  //ESP NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  //ESP NOW
	nunChuk.begin();
	while (!nunChuk.connect()) {
		Serial.println("Nunchuk not detected!");
		delay(1000);
	}
}

void loop() {	
  //Get new data from nunchuk
	boolean success = nunChuk.update();

	if (!success) {
		Serial.println("Controller disconnected!");
    delay(1000);
	}

	else {
    //Update nunchuck struct
		UpdateAnalogStick();
    UpdateButtons();

    //Print values
    PrintValues();   

    //Send values to esp32
    esp_now_send(broadcastAddress, (uint8_t *)&structNunChuk, sizeof(structNunChuk));
	}

  PrintValues();
}

void PrintValues(){
  Serial.print(structNunChuk.joyX); Serial.print(" ");
  Serial.print(structNunChuk.joyY); Serial.print(" ");
  Serial.print(structNunChuk.Z); Serial.print(" ");
  Serial.println(structNunChuk.c);
}