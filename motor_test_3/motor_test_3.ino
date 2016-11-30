#include <Servo.h>

const int turn_motor_pin = 2;
const int thrust_motor_pin = 3;
const int STOP = 1500;
const int MIN = 1250;
const int MAX = 1750;
//int estop = 7;
//int 

Servo turn_motor;
Servo thrust_motor;

void setup(){
  Serial.begin(9600);
  pinMode(turn_motor_pin, OUTPUT);
  pinMode(thrust_motor_pin, OUTPUT);
  turn_motor.attach(turn_motor_pin);
  thrust_motor.attach(thrust_motor_pin);
//  pinMode(e/stop, INPUT)
  while (!Serial);
  Serial.println("speed 1k to 2k, 1.5k neutral");
  // put your setup code here, to run once:

}

void loop() {

if (Serial.available())
{
  int speed = Serial.parseInt();
  if (speed >= 1000 && speed <= 2000)
  {
    turn_motor.writeMicroseconds(speed); // this stops
    thrust_motor.writeMicroseconds(speed);
  }
}

//tu/rn_motor_pin.writeMicroseconds(STOP); // this stops
//thrus/t_motor.writeMicroseconds(STOP);   // stops


//  turn_motor.write(90);
//  //thrust_motor.write(30);
//  
//  for(int i=0; i<30; ++i){
//    //turn_motor.write(i);
//    thrust_motor.write(i);
//    Serial.println(i);
//    delay(100);
//  }
//
//  for(int i=30; i>0; --i){
//    turn_motor.write(i);
//    //thrust_motor.write(i);
//    Serial.println(i);
//    delay(100);
//  }
}
