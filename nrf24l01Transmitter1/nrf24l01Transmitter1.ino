//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

char my_str[6] = "Hello";
int i = 0;
   
void setup()
{
  Serial.begin(9600);
  
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
  //int jsX = analogRead(A1);
  //Serial.println(jsX);

  i++;
  if(i % 2 == 0){
    my_str[3] = 'y';
  }
  else{
    my_str[3] = 'x';
  }
  //text.setCharAt(0, 'c');
  Serial.println(my_str);
  radio.write(&my_str, sizeof(my_str));
}
