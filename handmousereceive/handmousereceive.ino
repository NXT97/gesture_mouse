/*
 * made using leonardo ,bluetooth connected to 101, both have bluetooth on 0,1 pins using serial1. if hand tilted right stops
 */
#include "Mouse.h"
int x,y,z;         //x,y coordinate and z for switch
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = 3;    // resting threshold
int center = range / 2;       // resting position value
void setup() {
  Serial.begin(9600);
    Serial1.begin(9600);
  Mouse.begin();
  z=40; //so that mouse not move until receive
}
void loop()
{
  if(Serial1.available())
  {
    x=Serial1.parseInt();
    y=Serial1.parseInt();
    z=Serial1.parseInt();
    
  }
  Serial.print(x);
  Serial.print("   ");
  Serial.print(y);
  Serial.print("   ");
  Serial.print(z);
  Serial.println();
   int xReading = readAxis(x);
  int yReading = readAxis(y);

  // if the mouse control state is active, move the mouse:
  if (z<30) {
    Mouse.move(xReading, yReading, 0);
  }
  delay(responseDelay);
}
int readAxis(int reading) {
  // read the analog input:
 
  // if the output reading is outside from the
  // rest position threshold,  use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

