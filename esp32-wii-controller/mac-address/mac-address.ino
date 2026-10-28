/*
 *  ESP32 Get MAC Address Example
 *  Full Tutorial @ https://deepbluembedded.com/esp32-wifi-library-examples-tutorial-arduino/
 */
#include <WiFi.h>

void setup()
{
  Serial.begin(9600); // Set baud rate to 115200
  delay(1000);        // Add delay to allow Serial Monitor to connect
  Serial.println("HELLO");
  // Start WiFi
  WiFi.mode(WIFI_STA); // Set WiFi to station mode
  WiFi.begin();        // Initialize the WiFi
  delay(1000);         // Allow some time for the WiFi to initialize
  Serial.println("HELLO 2");
  // Print MAC Address
  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop()
{
  Serial.println(WiFi.macAddress());
}