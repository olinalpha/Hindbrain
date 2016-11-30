#include <Servo.h>

const int turn_motor_pin = 3;
const int thrust_motor_pin = 2;
const int STOP = 1500;
const int MIN = 1250;
const int MAX = 1750;

int SPEED = STOP;
int SPEED_NOW = STOP;
const int DELTA_SPEED = 5;

int eStopPin = 7;
unsigned long time1;
unsigned long time2;

Servo turn_motor;   // controls the turning part of the roboclaw
Servo thrust_motor; // controls the speed part of the roboclaw

void setup() {
  Serial.begin(9600);
  pinMode(turn_motor_pin, OUTPUT);
  pinMode(thrust_motor_pin, OUTPUT);
  pinMode(eStopPin, INPUT);
  turn_motor.attach(turn_motor_pin);
  thrust_motor.attach(thrust_motor_pin);

  thrust_motor.writeMicroseconds(SPEED);
  //  pinMode(e/stop, INPUT)
  //  while (!Serial);
  //  Serial.println("speed 1k to 2k, 1.5k neutral");
  // put your setup code here, to run once:

}

void loop() {

  if (Serial.available())
  {
    time1 = millis();
    SPEED = Serial.parseInt();
    Serial.println(SPEED);
  }


  if (SPEED >= 1000 && SPEED <= 2000)
  {
    if (SPEED_NOW < SPEED) {
      SPEED_NOW += DELTA_SPEED;
      thrust_motor.writeMicroseconds(SPEED_NOW);
      delay(1000);
      Serial.println(SPEED_NOW);
    }
    if (SPEED_NOW > SPEED) {
      SPEED_NOW -= DELTA_SPEED;
      thrust_motor.writeMicroseconds(SPEED_NOW);
      delay(1000);
      Serial.println(SPEED_NOW);
    }
  }


  if (readEstop() == 0)
    {
      SPEED = 1500;
      SPEED_NOW = 1500;
      Serial.println("estop pressed");
    }

  //turn_motor_pin.writeMicroseconds(STOP); // this stops
  //thrust_motor.writeMicroseconds(STOP);   // stops
}

boolean readEstop() {
  boolean eStopTriggered = digitalRead(eStopPin);
  return eStopTriggered;
}

