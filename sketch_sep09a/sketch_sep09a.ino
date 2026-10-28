const int JSPX = A0;
const int JSPY = A1;
const int JSPS = 2;

int jsX;
int jsY;

void setup(){
  Serial.begin(9600);
}

void loop(){
  UpdateJS();  
  
  Serial.println(jsX);
}

void UpdateJS(){  
  jsX = analogRead(JSPX);
  jsY = analogRead(JSPY);
  jsS = digitalRead(JSPS);
  
  if(jsX > 900){
    //jsDir[0] = -1;
    //jsDir[1] = 0;

    jsX = -1;
    jsY = 0;
  }

  else if(jsX < 123){
    //jsDir[0] = 1;
    //jsDir[1] = 0;

    jsX = 1;
    jsY = 0;
  }

  else if(jsY > 900){
    //jsDir[0] = 0;
    //jsDir[1] = 1;

    jsX = 0;
    jsY = 1;
  }

  else if(jsY < 123){
    //jsDir[0] = 0;
    //jsDir[1] = -1;

    jsX = 0;
    jsY = -1;
  }

  else{
    //jsDir[0] = 0;
    //jsDir[1] = 0;

    jsX = 0;
    jsY = 0;
  }
}
