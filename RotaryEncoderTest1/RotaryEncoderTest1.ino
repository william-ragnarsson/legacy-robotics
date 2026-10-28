#define outputA 2
#define outputB 3
#define outputC 4

 int counter = 0; 
 int aState;
 int aLastState;  

 void setup() { 
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
   pinMode(outputC, INPUT);

  digitalWrite(outputC, HIGH);
   
   Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);   
 } 

 void loop() {
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occureds   
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }
   } 

   if(digitalRead(outputC) == 0){
    Serial.println(counter); 
   }
   aLastState = aState; // Updates the previous state of the outputA with the current state
 }
