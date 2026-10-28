#include "my_library.h"

//Main Menu
void ClickButton(StructJoystick *S){

}

//AchtungDieKurve
bool matrix[128][160] = {0};

StructKurve resetKurve;

void UpdateKurveAngle(StructKurve *S, StructJoystick *js){
  S->angle += (js->left - js->right) * S->turnSpeed;
}

void UpdateKurvePos(StructKurve *S){
  S->x += cos(S->angle / 360 * PI) * S->moveSpeed;
  S->y -= sin(S->angle / 360 * PI) * S->moveSpeed;
}

bool IsKurveCollision(StructKurve *S){
  //Check for general collision;
  if((S->intX != S->prevX or S->intY != S->prevY) and matrix[S->intY][S->intX]){
    return true;
  }
  //Check for out of bounds
  else if(S->intX < 0 or S->intY < 0 or S->intX > 159 or S->intY > 127){
    return true;
  }
  //Check for upward diagonal collision
  else if(S->prevY - 1 == S->intY){
    if(S->prevX + 1 == S->intX      and     matrix[S->intY + 1][S->intX] and matrix[S->intY][S->intX - 1]){
      return true;
    }
    else if(S->prevX - 1 == S->intX     and     matrix[S->intY + 1][S->intX] and matrix[S->intY][S->intX + 1]){
      return true;
    }
  }
  //Check for downard diagonal collision
  else if(S->prevY + 1 == S->intY){
    if(S->prevX + 1 == S->intX      and     matrix[S->intY - 1][S->intX] and matrix[S->intY][S->intX - 1]){
      return true;
    }
    else if(S->prevX - 1 == S->intX     and     matrix[S->intY - 1][S->intX] and matrix[S->intY][S->intX + 1]){
      return true;
    }
  }
  return false;
}

void UpdateKurveIntVals(StructKurve *S){
  S->intX = int(S->x);
  S->intY = int(S->y);
}

void UpdateKurvePrevPos(StructKurve *S){
  S->prevX = S->intX;
  S->prevY = S->intY;
}

void UpdateKurveMatrix(StructKurve *S){
  matrix[S->intY][S->intX] = true;
}

bool LeaveKurveGap(StructKurve *S){
  S->travelDist++;

  if(S->travelDist <= 250) return false;
  else if(S->travelDist >= 270) S->travelDist = 0;
  return true;
}