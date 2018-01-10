#include <ZumoMotors.h>
ZumoMotors motors;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char val = Serial.read();

  if(val == 'w') {
    motors.setSpeeds(100, 100);
  }

  if(val == 'x') {
    motors.setSpeeds(-100, -100);
  }

  if(val == 'd') {
    motors.setSpeeds(100, 0);
  }

  if(val ==  'a') {
    motors.setSpeeds(0, 100);
  }

  if(val == 's') {
    motors.setSpeeds(0, 0);
  }
}
