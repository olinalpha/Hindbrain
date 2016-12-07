#include <Servo.h>
#include <ros.h>
//#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_TiCoServo.h>

ros::NodeHandle nh;

const int MOTOR_L_pin = 2;
const int MOTOR_R_pin = 3;
const int STOP = 1570; // note biased nuetral position-- hard turning speeds must be above 1570
const int MIN = 1500; 
int MAX = 1700;


float angular  = 0.0;
float linear = 0.0;
int motorPower = STOP;

int eStopPin = 7;
int leftMotorValue = 1570;
int rightMotorValue = 1570;
int prevTime = 0;
int rampDelay = 1;
Adafruit_TiCoServo MOTOR_L;   
Adafruit_TiCoServo MOTOR_R; 

// lights stuff
int LstickPin= 4;  // update to include the correct pins
int RstickPin= 5;
int LringPin = 6;
int RringPin = 8;

// front lights
Adafruit_NeoPixel  FrontLeft = Adafruit_NeoPixel(6, LringPin);
Adafruit_NeoPixel  FrontRight = Adafruit_NeoPixel(6, RringPin);
// back lights
Adafruit_NeoPixel  BackLeft = Adafruit_NeoPixel(6, LstickPin);
Adafruit_NeoPixel  BackRight = Adafruit_NeoPixel(6, RstickPin);


void CMD_VEL(const geometry_msgs::Twist& cmd_msg){
   angular = cmd_msg.angular.z;
   linear = cmd_msg.linear.x;   
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", CMD_VEL);

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_L_pin, OUTPUT);
  pinMode(MOTOR_R_pin, OUTPUT);
  pinMode(eStopPin, INPUT);
  MOTOR_L.attach(MOTOR_L_pin);
  MOTOR_R.attach(MOTOR_R_pin);

  MOTOR_L.writeMicroseconds(STOP); 
  MOTOR_R.writeMicroseconds(STOP);
  nh.subscribe(sub);
  nh.initNode();
  
//  Serial.println('test');
  delay(5);
  prevTime = millis();
}

void loop() {
  
  nh.spinOnce();
  
//Check if the Estop is pressed
  if (readEstop() == 0){ //Should change this to WHILE loop and assign STOP to R and L motors
      //MODE = 0;
      //MOTOR_L.writeMicroseconds(STOP);
      //MOTOR_R.writeMicroseconds(STOP);
      Serial.println("estop pressed");
      
   }else{

   
   }

  MAX = STOP + (linear * 300/2);  
  motorPower = MAX  - (abs(angular) * 300);
  

  if (angular < 0){
    ramp_speed(motorPower, MAX);  // this is turning left
    // lights
    FrontLeftTurnLight(1);
    BackLeftTurnLight(1);
    FrontRightTurnLight(2);
    BackLeftTurnLight(2);
  }
  else if (angular > 0){    
    ramp_speed(MAX, motorPower);
    // lights
    FrontLeftTurnLight(2);
    BackLeftTurnLight(2);
    FrontRightTurnLight(1);
    BackRightTurnLight(1);
  }
  else{ // angle is 0
    ramp_speed(MAX, MAX);
    // lights
    FrontLeftTurnLight(2);
    BackLeftTurnLight(2);
    FrontRightTurnLight(2);
    BackRightTurnLight(2);
  }

}

boolean readEstop() {
  boolean eStopTriggered = digitalRead(eStopPin);
  return eStopTriggered;
}

void writemix(int& leffMotorValue, int& rightMotorValue){
  MOTOR_L.writeMicroseconds(leftMotorValue + rightMotorValue/2);
  MOTOR_R.writeMicroseconds(leftMotorValue - rightMotorValue/2);
}

void ramp_speed(int speedLeft, int speedRight)
{
  if(millis() - prevTime > rampDelay){
    if (speedLeft > leftMotorValue){
      leftMotorValue ++;
    }else if (speedLeft < leftMotorValue){
      leftMotorValue --;
    }
    if (speedRight > rightMotorValue){
      rightMotorValue ++;
    }else if (speedRight < rightMotorValue){
      rightMotorValue --;
    }
    prevTime = millis();
  }
  MOTOR_L.writeMicroseconds(leftMotorValue);
  MOTOR_R.writeMicroseconds(rightMotorValue);
  
}

void FrontLeftTurnLight(int color_value)
  // 1 = red
  // 2 = off
  // 3 = High Beams
  {
    int i;
    if(color_value = 1)
      {
        for(i=0; i < FrontLeft.numPixels(); i++)
          {
            FrontLeft.setPixelColor(i, 255, 0, 0);
          }
        FrontLeft.show();
      }
    else if(color_value = 2)
      {
        FrontLeft.clear();
      }
   else if(color_value = 3)
      {
        for(i=0; i < FrontLeft.numPixels(); i++)
          {
            FrontLeft.setPixelColor(i, 255, 255, 255);
          }
        FrontLeft.show();
      }
  }

void FrontRightTurnLight(int color_value)
  // 1 = red
  // 2 = off
  // 3 = High Beams
  {
    int i;
    if(color_value = 1)
      {
        for(i=0; i < FrontRight.numPixels(); i++)
          {
            FrontRight.setPixelColor(i, 255, 0, 0);
          }
        FrontRight.show();
      }
    else if(color_value = 2)
      {
        FrontRight.clear();
      }
    else if(color_value = 3)
      {
        for(i=0; i < FrontRight.numPixels(); i++)
          {
            FrontRight.setPixelColor(i, 255, 255, 255);
          }
        FrontRight.show();
      }
  }
  
void BackLeftTurnLight(int color_value)
  // 1 = red
  // 2 = off
  {
    int i;
    if(color_value = 1)
      {
        for(i=0; i < BackLeft.numPixels(); i++)
          {
            BackLeft.setPixelColor(i, 255, 0, 0);
          }
        BackLeft.show();
      }
    else if(color_value = 2)
      {
        BackLeft.clear();
      }

  }

void BackRightTurnLight(int color_value)
  // 1 = red
  // 2 = off
  {
    int i;
    if(color_value = 1)
      {
        for(i=0; i < BackRight.numPixels(); i++)
          {
            BackRight.setPixelColor(i, 255, 0, 0);
          }
        BackRight.show();
      }
    else if(color_value = 2)
      {
        BackRight.clear();
      }
  }


