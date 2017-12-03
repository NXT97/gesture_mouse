/*
 * made using leonardo ,bluetooth connected to 101, both have bluetooth on 0,1 pins using serial1. if hand tilted right stops
 * 
 */
#include <CurieIMU.h>
#include <MadgwickAHRS.h>
int i=0;
int a,b,x,y;
Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  // start the IMU and filter
  CurieIMU.begin();
  CurieIMU.setGyroRate(25);
  CurieIMU.setAccelerometerRate(25);
  filter.begin(25);
  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
  CurieIMU.setGyroRange(250);
  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
}

void loop() {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    // read raw data from CurieIMU
    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);

    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    /*Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);*/

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  
  if (i==0)     // when calibrating for first time
  {
    a=heading; 
    b=pitch;
    i=1;
  }
  x=heading-a;
  y=pitch-b;
  x=map(x,-13,13,1366,0);  
  x=max(x,0);
  x=min(x,1366);
  y=map(y,-13,13,0,766);  
  y=max(y,0);
  y=min(y,766);
  Serial1.print(1400);
  Serial1.print(",");
  Serial1.print(x);
  Serial1.print(",");
  Serial1.print(y);
  Serial1.println();
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.println();
}

}

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
/*
 *add better on off system
 *add left and right click option using finger flex sensors
  */
