#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H
//Arduino include
#include <Arduino.h>
//ESP NOW
#include <esp_now.h>
#include <WiFi.h>

// Joystick

typedef struct StructJoystick {
  int up, down, right, left;

  // int upClick, downClick, rightClick, leftClick;
};

//Main Menu

void ClickButton(StructJoystick *S);

//Achtung Die Kurve
extern bool matrix[128][160];

typedef struct StructKurve{
  float angle, x, y;

  int intX, intY, prevX, prevY;

  float moveSpeed = 1;
  float turnSpeed = 10;
  int travelDist = 0;
};

void UpdateKurveAngle(StructKurve *S, StructJoystick *js);
void UpdateKurvePos(StructKurve *S);

void UpdateKurveIntVals(StructKurve *S);
void UpdateKurvePrevPos(StructKurve *S);

void UpdateKurveMatrix(StructKurve *S);
bool IsKurveCollision(StructKurve *S);
bool LeaveKurveGap(StructKurve *S);
#endif