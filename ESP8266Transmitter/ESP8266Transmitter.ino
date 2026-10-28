//MAC address 8C:AA:B5:7F:99:61

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0xB8, 0xD6, 0x1A, 0x44, 0x3B, 0x4C };

// Structure example to send data
// Must match the receiver structure
struct struct_message {
  char a_char[32];
  int b_int;
  float c_float;
  bool d_bool;
};

// Create a struct_message called myData
struct_message myData;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success\n");
  } else {
    Serial.println("Delivery fail\n");
  }
}

void setup() {
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  // Set values to send
  strcpy(myData.a_char, "THIS IS A CHAR");
  myData.b_int = random(1, 20);
  myData.c_float = (float)random(10.0, 100.0) / 10.0;
  myData.d_bool = random(1, 10) < 5 ? true : false;

  // Send message via ESP-NOW
  Serial.print("Float: ");
  Serial.println(myData.c_float);

  // Allow data to get out of the UART
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  delay(1000);
}