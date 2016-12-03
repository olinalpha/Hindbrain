#include <Servo.h>
#include <ros.h>
//#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>


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

Servo MOTOR_L;   
Servo MOTOR_R; 



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
}

void loop() {
  
  nh.spinOnce();
  
//Check if the Estop is pressed
  if (readEstop() == 0){ //Should change this to WHILE loop and assign STOP to R and L motors
      //MODE = 0;
      MOTOR_L.writeMicroseconds(STOP);
      MOTOR_R.writeMicroseconds(STOP);
      Serial.println("estop pressed");
      
   }else{

   
   }

  MAX = STOP + (linear * 300);  
  motorPower = MAX  - (abs(angular) * 300);

  if (angular < 0){       
    MOTOR_L.writeMicroseconds(motorPower);
    MOTOR_R.writeMicroseconds(MAX);
  }
  else if (angular > 0){    
    MOTOR_L.writeMicroseconds(MAX);
    MOTOR_R.writeMicroseconds(motorPower);
  }
  else{ // angle is 0
    MOTOR_L.writeMicroseconds(MAX);
    MOTOR_R.writeMicroseconds(MAX);
  }

}

boolean readEstop() {
  boolean eStopTriggered = digitalRead(eStopPin);
  return eStopTriggered;
}

//
//int ramp_speed(int SPEED_NOW,int  SPEED, Servo MOTOR)
//{
////  int SPEED_NOW;
//  if (SPEED_NOW < SPEED) {
//      SPEED_NOW += DELTA_SPEED;
//      MOTOR.writeMicroseconds(SPEED_NOW);
//      Serial.println(SPEED_NOW);
//    }
//    if (SPEED_NOW > SPEED) {
//      SPEED_NOW -= DELTA_SPEED;
//      MOTOR.writeMicroseconds(SPEED_NOW);
//      Serial.println(SPEED_NOW);
//    }
//  return SPEED_NOW;
//}


