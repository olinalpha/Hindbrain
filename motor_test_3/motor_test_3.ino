#include <Servo.h>

const int turn_motor_pin = 2;
const int thrust_motor_pin = 3;

Servo turn_motor;
Servo thrust_motor;

void setup(){
  Serial.begin(9600);
  pinMode(turn_motor_pin, OUTPUT);
  pinMode(thrust_motor_pin, OUTPUT);
  turn_motor.attach(turn_motor_pin);
  thrust_motor.attach(thrust_motor_pin);


  // put your setup code here, to run once:

}

void loop() {

  turn_motor.write(90);
  //thrust_motor.write(30);
  
  for(int i=0; i<30; ++i){
    //turn_motor.write(i);
    thrust_motor.write(i);
    Serial.println(i);
    delay(100);
  }

  for(int i=30; i>0; --i){
    turn_motor.write(i);
    //thrust_motor.write(i);
    Serial.println(i);
    delay(100);
  }
}
