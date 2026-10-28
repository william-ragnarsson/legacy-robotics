#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "EstieLars12n";
const char* password = "Ginko_b21";

void setup(){
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected");
}

void loop(){
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin("https://jsonplaceholder.typicode.com/comments?id=10");
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("ERROR");
    }

    http.end();
  }

  delay(5000);
}