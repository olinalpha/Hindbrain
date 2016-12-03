#include <Servo.h>

const int MOTOR_L_pin = 2;
const int MOTOR_R_pin = 3;
const int STOP = 1570; // note biased nuetral position-- hard turning speeds must be above 1570
const int MIN = 1500; 
const int MAX = 1700;
int timer = 0; 

//   MODES         0     1     2     3     4     5    6    7    8
int SPEED_L[9] = {STOP, MAX,  MAX,  MAX, 1650, 1600, MIN, MIN, 1520};
int SPEED_R[9] = {STOP, 1600, 1650, MAX, MAX,  MAX, 1520, MIN, MIN};

int MODE = 0; //Initialized Mode = 0 is STOP setting for motors
int SPEED_NOW_L = STOP;
int SPEED_NOW_R = STOP;
const int DELTA_SPEED = 5;

int eStopPin = 7;

unsigned long time1 = 0;    // will store the last time motor speeds were updated (milliseconds)
unsigned long time2 = 0;    // current time (milliseconds)
const long ramp_time = 100; // wait time for mechanical system to change speeds (milliseconds)

Servo MOTOR_L;   
Servo MOTOR_R; 

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_L_pin, OUTPUT);
  pinMode(MOTOR_R_pin, OUTPUT);
  pinMode(eStopPin, INPUT);
  MOTOR_L.attach(MOTOR_L_pin);
  MOTOR_R.attach(MOTOR_R_pin);

  MOTOR_L.writeMicroseconds(SPEED_L[MODE]); 
  MOTOR_R.writeMicroseconds(SPEED_R[MODE]);

}

void loop() {

//Check if the Estop is pressed
  if (readEstop() == 0){ //Should change this to WHILE loop and assign STOP to R and L motors
      //MODE = 0;
      MOTOR_L.writeMicroseconds(STOP); // Should probably replace 0 with MODE... same thing in setup but more flexible
      MOTOR_R.writeMicroseconds(STOP);
      SPEED_NOW_L = STOP;
      SPEED_NOW_R = STOP;
      MODE = 0;
      Serial.println("estop pressed");
      
   }else{

   
   }

//Check if new input from serial monitor; will be replaced with ROS update
  if (Serial.available()){
    
    MODE = Serial.parseInt();
    Serial.println(MODE);
  }
  
 
  time1 = millis();
//Check if speed needs to be changed (new mode)
  if (time1-time2>=ramp_time){
    time2=time1;
  
    if (SPEED_NOW_L != SPEED_L[MODE]){
      SPEED_NOW_L =  ramp_speed(SPEED_NOW_L, SPEED_L[MODE], MOTOR_L);
    
    }
    if (SPEED_NOW_R != SPEED_R[MODE]){
      SPEED_NOW_R =  ramp_speed(SPEED_NOW_R, SPEED_R[MODE], MOTOR_R);
      
    }
  }

}

boolean readEstop() {
  boolean eStopTriggered = digitalRead(eStopPin);
  return eStopTriggered;
}

int ramp_speed(int SPEED_NOW,int  SPEED, Servo MOTOR)
{
//  int SPEED_NOW;
  if (SPEED_NOW < SPEED) {
      SPEED_NOW += DELTA_SPEED;
      MOTOR.writeMicroseconds(SPEED_NOW);
      Serial.println(SPEED_NOW);
    }
    if (SPEED_NOW > SPEED) {
      SPEED_NOW -= DELTA_SPEED;
      MOTOR.writeMicroseconds(SPEED_NOW);
      Serial.println(SPEED_NOW);
    }
  return SPEED_NOW;
}


