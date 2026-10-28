const int JSPX = A1;
const int JSPY = A0;

int jsX;
int jsY;

void UpdateSnakeDir(){
  jsX = analogRead(JSPX);
  jsY = analogRead(JSPY);

  if(jsX > 800){
    snakeDir[0] = -1;
    snakeDir[1] = 0;
  }

  else if(jsX < 200){
    snakeDir[0] = 1;
    snakeDir[1] = 0;
  }

  else if(jsY > 700){
    snakeDir[1] = 1;
    snakeDir[0] = 0;
  }
  
  else if(jsY < 200){
    snakeDir[1] = -1;
    snakeDir[0] = 0;
  }
}
