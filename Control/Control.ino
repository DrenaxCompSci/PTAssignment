#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <Pushbutton.h>

ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;
Pushbutton button(ZUMO_BUTTON);

const int speed = 50;
const int turnSpeed = 200;
const int turnDuration = 500; //0.5 seconds

const int corridorThreshold = 140;

void setup() {
  // Initialise the reflectance module
  reflectanceSensors.init();

  // Delay 3 seconds
  delay(3000);

  // LED to alert calibration mode is active
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  for(int i = 0; i < 80; i++){
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70)) {
      motors.setSpeeds(-(turnSpeed), turnSpeed);
    } else {
      motors.setSpeeds(turnSpeed, -(turnSpeed));
    }
    reflectanceSensors.calibrate();
    delay(20);
  }

  // Stop Zumo and turn off LED now calibration mode is no longer active
  motors.setSpeeds(0, 0);
  digitalWrite(13, LOW);

  button.waitForButton();

  // Begin communication
  Serial.begin(9600);
}

void loop() {
  unsigned int sensors[6];
//
//  int position = reflectanceSensors.readLine(sensors);
  reflectanceSensors.read(sensors);

  for (byte i = 0; i < 6; i++) {
    Serial.print(sensors[i]);
    Serial.print(' ');
  }

  Serial.println("");

  if (sensors[0] > corridorThreshold){
    motors.setSpeeds(-(speed), -(speed));
    delay(turnDuration);
    motors.setSpeeds(turnSpeed, -(turnSpeed));
    delay(turnDuration);
    motors.setSpeeds(speed, speed);
  } else if (sensors[5] > corridorThreshold) {
    motors.setSpeeds(-(speed), -(speed));
    delay(turnDuration);
    motors.setSpeeds(-(turnSpeed), turnSpeed);
    delay(turnDuration);
    motors.setSpeeds(speed, speed);
    } 
    //else {
//    motors.setSpeeds(speed, speed);
//  }

  // Read in user value for the current command
  char val = Serial.read();

  // Forwards
  if(val == 'w') {
    motors.setSpeeds(speed, speed);
  }

  // Backwards
  if(val == 'x') {
    motors.setSpeeds(-(speed), -(speed));
  }

  // Right
  if(val == 'd') {
    motors.setSpeeds(turnSpeed, 0);
  } 

  // Left
  if(val ==  'a') {
    motors.setSpeeds(0, turnSpeed);
  }

  // Stop
  if(val == 's') {
    motors.setSpeeds(0, 0);
  }
}
