#include <Adafruit_TiCoServo.h>
#include <ros.h>
//#include <std_msgs/String.h>
#include <Adafruit_NeoPixel.h>
#include <geometry_msgs/Twist.h>
ros::NodeHandle nh;


#define MOTOR_L_pin 2
#define MOTOR_R_pin 3
#define eStopPin 7
#define headlightLeftPin 14
#define headlightRightPin 15
#define taillightLeftPin 8
#define taillightRightPin 9


//Motor Variables
const int neutralAccel = 1470;
const int maxRange = 800;
float angularVel = 0;
float linearVel = 0;
int leftMotorValue = 1470;
int rightMotorValue = 1470;
int prevTime = 0;
const int rampDelay = 2;
const int velScale = 150;
const int turnScale = 150;


//Light stuff
Adafruit_NeoPixel headlightLeft = Adafruit_NeoPixel(12, headlightLeftPin, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel headlightRight = Adafruit_NeoPixel(12, headlightRightPin, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel taillightLeft = Adafruit_NeoPixel(8, taillightLeftPin, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel taillightRight = Adafruit_NeoPixel(8, taillightRightPin, NEO_GRBW + NEO_KHZ800);

int brightness = 20;

uint32_t red = headlightLeft.Color(255, 0, 0);
uint32_t blue = headlightLeft.Color(0, 0, 255);
uint32_t green = headlightLeft.Color(0, 255, 0);
uint32_t white = headlightLeft.Color(255, 255, 255);
uint32_t yellow = headlightLeft.Color(255, 100, 0);
uint32_t off = headlightLeft.Color(0, 0, 0);
unsigned long prevTimeLightsBlink = 0;
unsigned long prevTimeLightsRipple = 0;
unsigned long currentMillis = 0;
int rippleIndex = 0;


Adafruit_TiCoServo MOTOR_L;  
Adafruit_TiCoServo MOTOR_R;


void CMD_VEL(const geometry_msgs::Twist& cmd_msg){
   angularVel = cmd_msg.angular.z;
   linearVel = cmd_msg.linear.x;   
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", CMD_VEL);

void setup() {
  Serial.begin(19200);
  pinMode(MOTOR_L_pin, OUTPUT);
  pinMode(MOTOR_R_pin, OUTPUT);
  pinMode(eStopPin, INPUT);
  MOTOR_L.attach(MOTOR_L_pin, neutralAccel - maxRange/2, neutralAccel + maxRange/2);
  MOTOR_R.attach(MOTOR_R_pin, neutralAccel - maxRange/2, neutralAccel + maxRange/2);
  
  headlightLeft.begin();
  headlightRight.begin();
  taillightLeft.begin();
  taillightRight.begin();
  showLights();
  headlightLeft.setBrightness(brightness);
  headlightRight.setBrightness(brightness);
  taillightLeft.setBrightness(brightness);
  taillightRight.setBrightness(brightness);

  MOTOR_L.write(90); 
  MOTOR_R.write(90);
  //nh.subscribe(sub);
  //nh.initNode();
  
  Serial.println("beginning");
  delay(5);
  currentMillis = millis();
  prevTime = millis();
  prevTimeLightsBlink = millis();
  prevTimeLightsRipple = millis();
}

void loop() {
  
  nh.spinOnce();
  currentMillis = millis();
  
//Check if the Estop is pressed
  if (readEstop() == 0){ //Should change this to WHILE loop and assign STOP to R and L motors
      EStopped = true;
      //MODE = 0;
      //MOTOR_L.writeMicroseconds(STOP);
      //MOTOR_R.writeMicroseconds(STOP);      
   }else{
      EStopped = false;
      calculateMotorSpeed(.2, .2);
      //calculateMotorSpeed(linearVel, angularVel);
      rampSpeed(leftMotorValue, rightMotorValue);
      runLights();
      //Serial.println(leftMotorValue);
      //Serial.println(rightMotorValue);
   }
}

void runLights(){
  updateRippleIndex(500);
  setBlinking(taillightLeft, 12, red, 500);
  setRipple(taillightRight, 12, red);
  setRipple(headlightLeft, 12, green);
  setBlinking(headlightRight, 12, red, 500);
  //setSolid(headlightLeft, 12, red);
  //setSolid(headlightRight, 12, white);
  //setSolid(taillightLeft, 8, yellow);
  showLights();
}


boolean readEstop() {
  boolean eStopTriggered = digitalRead(eStopPin);
  return eStopTriggered;
}

void calculateMotorSpeed(float linear, float angular){
  //calculates motor speed from linear and angular
  leftMotorValue = linear*velScale - angular*turnScale;
  rightMotorValue = linear*velScale + angular*turnScale;
}


void setSolid(Adafruit_NeoPixel &light, int numPins, uint32_t color){
  for(int i = 0; i<numPins; i++){
    light.setPixelColor(i, color);
  }
}
void setBlinking(Adafruit_NeoPixel &light, int numPins, uint32_t color, unsigned long delayTime){
  /*if(currentMillis - prevTimeLightsBlink > delayTime){
    prevTimeLightsBlink = currentMillis;
    Serial.println("test1");
    Serial.println(currentMillis);
    Serial.println(prevTimeLightsBlink);
    Serial.println(currentMillis - prevTimeLightsBlink);
  }else if(currentMillis - prevTimeLightsBlink > delayTime/2){
    setSolid(light, numPins, off);
    Serial.println("test2");
    Serial.println(currentMillis);
    Serial.println(prevTimeLightsBlink);
    Serial.println(currentMillis - prevTimeLightsBlink);
  }else{
    setSolid(light, numPins, color);
    Serial.println("test3");
    Serial.println(currentMillis);
    Serial.println(prevTimeLightsBlink);
    Serial.println(currentMillis - prevTimeLightsBlink);
  }*/
  
  
  if((currentMillis - prevTimeLightsBlink) < delayTime/2){
    setSolid(light, numPins, color);
    Serial.println("test1");
    Serial.println(currentMillis);
    Serial.println(prevTimeLightsBlink);
  }else if((currentMillis - prevTimeLightsBlink) < delayTime){
    setSolid(light, numPins, off);
    Serial.println("test2");
    Serial.println(currentMillis);
    Serial.println(prevTimeLightsBlink);
  }else{
    prevTimeLightsBlink = currentMillis;
    Serial.println("test3");
    
  }/*
  if((currentMillis - prevTimeLightsBlink < -1) || (currentMillis - prevTimeLightsBlink >= delayTime)){
    prevTimeLightsBlink = currentMillis;
    Serial.println("test3");
    Serial.println(currentMillis);
    Serial.println(prevTimeLightsBlink);
    Serial.println(currentMillis - prevTimeLightsBlink);
  }else{
    Serial.println("test4:");
    Serial.println(currentMillis - prevTimeLightsBlink);
  }*/
}
void setRipple(Adafruit_NeoPixel &light, int numPins, uint32_t color){
  if(rippleIndex == -1){
    for(int i= 0; i < 12; i++){
       light.setPixelColor(i, off);
    }
  }else{
    light.setPixelColor(rippleIndex, color);
  }
}
void updateRippleIndex(unsigned long delayTime){
  if(currentMillis - prevTimeLightsRipple > delayTime/13){
    rippleIndex ++;
    if(rippleIndex > 11){
      rippleIndex = -1;
    }
    prevTimeLightsRipple = currentMillis;
  }
}
  
void rampSpeed(int speedLeft, int speedRight)
{
  if(currentMillis - prevTime > rampDelay){
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
    prevTime = currentMillis;
  }
  MOTOR_L.write(mapMotor(leftMotorValue));
  MOTOR_R.write(mapMotor(rightMotorValue));
  
}
int mapMotor(float input){
  return map(input,-(turnScale + velScale), turnScale + velScale, 0, 180);
}

void showLights(){
  headlightLeft.show();
  headlightRight.show();
  taillightLeft.show();
  taillightRight.show();
}

