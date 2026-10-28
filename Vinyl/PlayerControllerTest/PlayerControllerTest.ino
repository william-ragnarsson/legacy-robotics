#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>

#include <ArduinoJson.h>

char ssid[] = "EstieLars12n";
char password[] = "Ginko_b21";

char clientId[] = "0fb5a343a9224ce78f21ca10df26927a";
char clientSecret[] = "65044bde180d4567bcbc2888376b8fe1";

#define SPOTIFY_MARKET "BE"
#define SPOTIFY_REFRESH_TOKEN "AQBy1j4qp-U4AfF2omKTcH72lZQ_cIdB9g8TsfCDXCiWlyLVxX3Fg7Ck95xaBIFbPO3wh0HcRg4Awb8rtiJ8ZW-moDqwSu18pdN82kMqKtsEaVJZtJEzxh6_ACN-PITsTTo"
 
//------- ---------------------- ------

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

void playSingleTrack()
{ 
    char sampleTrack[] = "spotify:track:1icgLGTpX2fQXKRe4D7w2b";
    
    char body[100];
    sprintf(body, "{\"uris\" : [\"%s\"]}", sampleTrack);
    spotify.playAdvanced(body);
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);

    client.setCACert(spotify_server_cert);

    Serial.println("Refreshing Access Tokens");
    if (!spotify.refreshAccessToken())
    {
        Serial.println("Failed to get access tokens");
        return;
    }

    Serial.println("Playing Single Track");
    playSingleTrack();
}

void loop()
{
  playSingleTrack();
  delay(5000);
}
