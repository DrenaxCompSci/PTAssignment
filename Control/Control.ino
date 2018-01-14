#include <ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>
#include <Pushbutton.h>
#include <NewPing.h>

#define TRIGGER_PIN 6
#define ECHO_PIN 3
#define MAX_DISTANCE 30

ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;
Pushbutton button(ZUMO_BUTTON);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Motor variables
const int speed = 100;
const int turnSpeed = 150;
const int turnDuration = 500; //0.5 seconds

// Reflectance sensor variables
const int corridorThreshold = 140;
const int cornerThreshold = 80;

// Id/Mapping
int roomIdCount = 0;
int sideIdCount = 0;
int corridorIdCount = 0;

void setup() {
  // Initialise the reflectance module
  reflectanceSensors.init();

  // Delay 3 seconds
  Serial.println("Preparing to calibrate!");
  delay(3000);

  // LED to alert calibration mode is active
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.println("Calibrating...");
  calibration();

  // Stop Zumo and turn off LED now calibration mode is no longer active
  motors.setSpeeds(0, 0);
  digitalWrite(13, LOW);

  // Wait for Zumo button to be pressed
  button.waitForButton();

  // Begin communication
  Serial.begin(9600);
}

void loop() {
  // Read in reflectance sensors data
  unsigned int sensors[6];
  reflectanceSensors.read(sensors);

  // Read in user value for the current command
  char val = Serial.read();

  // Manual override
  if (val == 'v') {
    Serial.println("Manual Override initiated. Please direct into room or corridor and press 'b' to continue");
    val = 'p';  // Random value that isn't used for control control
    manualOverride(val);    // Rescursive function for side-corridors & rooms within
  }

  cornerCheck(val);
}

void calibration() {
  for (int i = 0; i < 80; i++) {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70)) {
      motors.setSpeeds(50 , 50);
    } else {
      motors.setSpeeds(-(50), -(50));
    }
    reflectanceSensors.calibrate();
    delay(20);
  }

  Serial.println("Calibration complete! Press Zumo button to start.");
}

void stayInCorridor() {
  unsigned int sensors[6];
  reflectanceSensors.read(sensors);
  
  if (sensors[0] > corridorThreshold) {
      motors.setSpeeds(-(speed / 2), -(speed / 2));
      delay(turnDuration);
      motors.setSpeeds(turnSpeed, -(turnSpeed));
      delay(turnDuration);
      motors.setSpeeds(speed, speed);
    } else if (sensors[5] > corridorThreshold) {
      motors.setSpeeds(-(speed / 2), -(speed / 2));
      delay(turnDuration);
      motors.setSpeeds(-(turnSpeed), turnSpeed);
      delay(turnDuration);
      motors.setSpeeds(speed, speed);
    } else {
      motors.setSpeeds(speed, speed);
    }
}

void manualControl(char val) {
  while (val != 'b') {
    val = Serial.read();

    // Forwards
    if (val == 'w') {
      motors.setSpeeds(speed, speed);
    }

    // Backwards
    if (val == 'x') {
      motors.setSpeeds(-(speed), -(speed));
    }

    // Right
    if (val == 'd') {
      motors.setSpeeds(turnSpeed, 0);
    }

    // Left
    if (val ==  'a') {
      motors.setSpeeds(0, turnSpeed);
    }

    // Stop
    if (val == 's') {
      motors.setSpeeds(0, 0);
    }
  }
}

void manualOverride(char val) {
    motors.setSpeeds(0, 0);
    manualControl(val);

    Serial.println("Please enter 'r' for room or 'c' for side-corridor");
    char roomOrCorridor = '0';

    // Wait for user input
    while (roomOrCorridor != 'r' && roomOrCorridor != 'c') {
      roomOrCorridor = Serial.read();
    }

    if (roomOrCorridor == 'r') {
      // Room
      checkRoom();
      motors.setSpeeds(0, 0);
      manualControl(val);
    } else {
      // Side-corridor
      Serial.print("Side corridor with ID: ");
      Serial.println(sideIdCount++);
      bool wallFlag = false;
      while (!(wallFlag)) {
          val = Serial.read();
          wallFlag = wallCheck();
          if (val == 'v') {
            val = 'p';
            manualOverride(val);
          }
                   
      }
      // Turn around 180 degrees
      motors.setSpeeds(-(speed), -(speed));
      delay(500);
      motors.setSpeeds(turnSpeed, -(turnSpeed));
      delay(1500);
      cornerCheck(val);
    }
}

void cornerCheck(char val) {
  unsigned int sensors[6];
  reflectanceSensors.read(sensors);
  
  // Stop at corner/manual control
  if (sensors[2] > cornerThreshold) {
    motors.setSpeeds(0, 0);
    Serial.println("Corner found! Manual Override initiated. Press 'b' to start automatic movement again");
    manualControl(val);
    Serial.print("Left corridor with ID: ");
    Serial.print(corridorIdCount++);
    Serial.println(", onwards!");
  } else {
    stayInCorridor();
  }
}

bool wallCheck() {
  unsigned int sensors[6];
  reflectanceSensors.read(sensors);
  
  if (sensors[2] > cornerThreshold) {
    motors.setSpeeds(0, 0);
    Serial.println("Wall found! Waiting for prompt (press b)...");
    char val = 'c';
    while(val != 'b'){
      val = Serial.read();  
    }    
    return true;
  } else {
    stayInCorridor();
    return false;
  }
}

void checkRoom() {
  Serial.println("Is the room on the left or right side of the corridor? (l or r)");
  char leftOrRight = '0';

  // Wait for user input
  while (leftOrRight != 'l' && leftOrRight != 'r') {
    leftOrRight = Serial.read();
  }

  // Display room ID
  Serial.print("Room with ID: ");
  int roomId = roomIdCount++;
  Serial.println(roomId);

  // Scan room
  bool found = false;

  motors.setSpeeds(turnSpeed, -(turnSpeed));
  found = sonarCheck(found, roomId);
  delay(turnDuration);

  motors.setSpeeds(-(turnSpeed), turnSpeed);
  found = sonarCheck(found, roomId);
  delay(2 * turnDuration);

  motors.setSpeeds(turnSpeed, -(turnSpeed));
  found = sonarCheck(found, roomId);
  delay(turnDuration);
}

bool sonarCheck(bool found, int roomId) {
  if (sonar.ping_cm() > 0 && !(found)) {
    Serial.print("Object found in room: ");
    Serial.println(roomId);
    found = true;
  }
  return found;
}

