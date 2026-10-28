/*int bikePos[2] = {0, 0};
int bikeDir[2] = {0, 0};
int lastBikeDir[2] = {0, 0};

void Tron(){
  UpdateBikeDir();
  UpdateBikePos();

  if(bikeDir[0] != 0 or bikeDir[1] != 0){
    UpdateMatrix();
  }
  
  DrawBike();

  lastBikeDir[0] = bikeDir[0];
  lastBikeDir[1] = bikeDir[1];
}

void UpdateBikeDir(){
  if(lastBikeDir[0] != -jsDir[0] ){
    bikeDir[0] = jsDir[0];
  }
  if(lastBikeDir[1] != -jsDir[1]){
    bikeDir[1] = jsDir[1];
  }
}

void UpdateBikePos(){
  bikePos[0] += bikeDir[0];
  bikePos[1] += bikeDir[1];
}

void DrawBike(){
  DrawRect(bikePos[0] * 2, bikePos[1] * 2, 2, 2, BLUE);
}

unsigned char PowerReturn(unsigned char num){
  unsigned char arr[8] = {128, 64, 32, 16, 8, 4, 2, 1};
  return arr[num];
}

void UpdateMatrix(){
  unsigned char modResultX = (bikePos[0] - (bikePos[0] % 8))/8;

  if ((matrix[bikePos[1]][modResultX] & (PowerReturn(bikePos[0] % 8))) > 0){
    Serial.println("COLLISION");
  }
  
  
  matrix[bikePos[1]][modResultX] |= PowerReturn(bikePos[0] % 8);
}

/*
void PrintEntireMatrix(){
  for(unsigned char i = 0; i < 20; i++){
    for(unsigned char j = 0; j < 10; j++){
      if(j != 9){
        Serial.print(matrix[i][j]);
        if(matrix[i][j] < 10){
          Serial.print("   ");
        }else if(matrix[i][j] < 99){
          Serial.print("  ");
        }else{
          Serial.print(" ");
        }
        
      }
      else{
        Serial.println(matrix[i][j]);
      }
    }

    if(i == 19){
      Serial.println("----------------------------");
    }
  }
}
*/
