#include <Servo.h>

const int MOTOR_L_pin = 2;
const int MOTOR_R_pin = 3;
const int STOP = 1570;
const int MIN = 1300;
const int MAX = 1700;
int timer = 0;

int SPEED_L[6] = {STOP, MAX,  MAX,  MAX, 1625, 1550};
int SPEED_R[6] = {STOP, 1550, 1625, MAX, MAX,  MAX};

int MODE = 0;
int SPEED_NOW_L = STOP;
int SPEED_NOW_R = STOP;
const int DELTA_SPEED = 5;

int eStopPin = 7;
unsigned long time1;
unsigned long time2;

Servo MOTOR_L;   
Servo MOTOR_R; 

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_L_pin, OUTPUT);
  pinMode(MOTOR_R_pin, OUTPUT);
  pinMode(eStopPin, INPUT);
  MOTOR_L.attach(MOTOR_L_pin);
  MOTOR_R.attach(MOTOR_R_pin);

  MOTOR_L.writeMicroseconds(SPEED_L[0]);
  MOTOR_R.writeMicroseconds(SPEED_R[0]);

}

void loop() {

  if (Serial.available())
  {
    time1 = millis();
    MODE = Serial.parseInt();
    Serial.println(MODE);
  }
if (SPEED_NOW_L != SPEED_L[MODE]){
SPEED_NOW_L =  ramp_speed(SPEED_NOW_L, SPEED_L[MODE], MOTOR_L);
}
if (SPEED_NOW_R != SPEED_L[MODE]){
SPEED_NOW_R =  ramp_speed(SPEED_NOW_R, SPEED_R[MODE], MOTOR_R);
}
  if (readEstop() == 0)
    {
      MODE = 0;
//      SPEED_NOW = 1500;
      Serial.println("estop pressed");
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
      delay(100);
      Serial.println(SPEED_NOW);
    }
    if (SPEED_NOW > SPEED) {
      SPEED_NOW -= DELTA_SPEED;
      MOTOR.writeMicroseconds(SPEED_NOW);
      delay(100);
      Serial.println(SPEED_NOW);
    }
  return SPEED_NOW;
}

