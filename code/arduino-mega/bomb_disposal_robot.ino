//====================================================//
//       ARDUINO MEGA + HC-05 + 2x BTS7960          //
//              4 x 12V Johnson Motors               //
//              + 5x Servo Robotic Arm               //
//====================================================//

#include <Servo.h>

//====================================================//
//                    HC-05                           //
//====================================================//
// Mega hardware Serial1
// Mega RX1 = D19
// Mega TX1 = D18
#define BT Serial1

//====================================================//
//              BTS7960 #1 - LEFT SIDE               //
//====================================================//
#define LEFT_RPWM 5
#define LEFT_LPWM 6

//====================================================//
//              BTS7960 #2 - RIGHT SIDE              //
//====================================================//
#define RIGHT_RPWM 7
#define RIGHT_LPWM 8

//====================================================//
//                 LIGHTS + HORN                     //
//====================================================//
#define FRONT_LIGHT_PIN 30
#define REAR_LIGHT_PIN  31
#define HORN_PIN        32

//====================================================//
//                 ROBOTIC ARM SERVOS                //
//====================================================//
// Pins changed to 2, 3, 4, 9, 10 to avoid conflict with BTS7960 (5, 6, 7, 8)
#define SERVO_BASE_PIN 2
#define SERVO_J2_PIN 3
#define SERVO_J1_PIN 4
#define SERVO_WRIST_PIN 9
#define SERVO_GRIP_PIN 10

Servo servoBase;
Servo servoJ2;
Servo servoJ1;
Servo servoWrist;
Servo servoGrip;

int currentBase = 110;
int currentJ2 = 90;
int currentJ1 = 145;
int currentWrist = 27;
int currentGrip = 110;

const int MOVE_DELAY = 15; // Smooth movement speed (Higher = Slower)

//====================================================//
//                 SPEED SETTINGS                    //
//====================================================//
int motorSpeed = 70;
int turnSpeed = 90;
int diagonalSpeed = 70;

//====================================================//
//                      SETUP                        //
//====================================================//
void setup() {
  //--------------- LEFT BTS7960 ----------------//
  pinMode(LEFT_RPWM, OUTPUT);
  pinMode(LEFT_LPWM, OUTPUT);

  //--------------- RIGHT BTS7960 ---------------//
  pinMode(RIGHT_RPWM, OUTPUT);
  pinMode(RIGHT_LPWM, OUTPUT);

  //--------------- Lights + Horn ---------------//
  pinMode(FRONT_LIGHT_PIN, OUTPUT);
  pinMode(REAR_LIGHT_PIN, OUTPUT);
  pinMode(HORN_PIN, OUTPUT);
  digitalWrite(FRONT_LIGHT_PIN, LOW);
  digitalWrite(REAR_LIGHT_PIN, LOW);
  digitalWrite(HORN_PIN, LOW);

  //--------------- Bluetooth -------------------//
  BT.begin(9600);
  Serial.begin(9600); // For PC debugging

  //--------------- Robotic Arm -----------------//
  servoBase.attach(SERVO_BASE_PIN);
  servoJ2.attach(SERVO_J2_PIN);
  servoJ1.attach(SERVO_J1_PIN);
  servoWrist.attach(SERVO_WRIST_PIN);
  servoGrip.attach(SERVO_GRIP_PIN);

  // Move to initial position on startup
  servoBase.write(currentBase);
  servoJ2.write(currentJ2);
  servoJ1.write(currentJ1);
  servoWrist.write(currentWrist);
  servoGrip.write(currentGrip);

  //--------------- Safety ----------------------//
  stopRobot();
  Serial.println("System Ready. Waiting for commands...");
}

//====================================================//
//                       LOOP                         //
//====================================================//
void loop() {
  if (BT.available()) {
    String cmd = BT.readStringUntil('\n');
    cmd.trim();

    if (cmd.length() > 0) {
      Serial.print("Received: ");
      Serial.println(cmd);

      //==========================================//
      //             ROBOTIC ARM CONTROL          //
      //==========================================//
      if (cmd.startsWith("A")) {
        int values[5];
        int startIndex = 2;
        
        for (int i = 0; i < 5; i++) {
          int endIndex = cmd.indexOf(',', startIndex);
          if (endIndex == -1) endIndex = cmd.length();
          values[i] = cmd.substring(startIndex, endIndex).toInt();
          startIndex = endIndex + 1;
        }
        
        stopRobot(); // Safety: ensure motors are off while arm moves
        
        Serial.println("Moving Arm Sequence...");
        smoothMove(servoBase, values[0], currentBase);
        smoothMove(servoJ2, values[1], currentJ2);
        
        // Invert J1 and Wrist to match physical mounting
        int targetJ1 = 290 - values[2]; 
        int targetWrist = 55 - values[3]; 
        
        smoothMove(servoJ1, targetJ1, currentJ1);
        smoothMove(servoWrist, targetWrist, currentWrist);
        
        // ⚠️ CRITICAL: values[4] (the gripper angle) is INTENTIONALLY IGNORED here.
        // The gripper will ONLY move when the standalone 'q' or 'Q' command is received.
        
        Serial.println("Arm Sequence Complete.");
      } 
      else if (cmd == "q") {
        Serial.println("Opening Gripper...");
        smoothMove(servoGrip, 110, currentGrip);
      }
      else if (cmd == "Q") {
        Serial.println("Closing Gripper...");
        smoothMove(servoGrip, 40, currentGrip);
      }
      
      //==========================================//
      //               CAR CONTROL                //
      //==========================================//
      else if (cmd.length() == 1) {
        char c = cmd.charAt(0);
        switch (c) {
          // Movement
          case 'F': forward(); break;
          case 'B': backward(); break;
          case 'L': left(); break;
          case 'R': right(); break;
          case 'S': stopRobot(); break;
          
          // Diagonal (Optional, kept for compatibility)
          case 'G': forwardLeft(); break;
          case 'I': forwardRight(); break;
          case 'Y': backLeft(); break;   // Changed from 'H' to avoid conflict with Horn
          case 'J': backRight(); break;

          // Lights (New App Protocol)
          case '1': digitalWrite(FRONT_LIGHT_PIN, HIGH); break;
          case '2': digitalWrite(FRONT_LIGHT_PIN, LOW); break;
          case '3': digitalWrite(REAR_LIGHT_PIN, HIGH); break;
          case '4': digitalWrite(REAR_LIGHT_PIN, LOW); break;
          
          // Lights (Old Protocol Compatibility)
          case 'W': digitalWrite(FRONT_LIGHT_PIN, HIGH); break;
          case 'w': digitalWrite(FRONT_LIGHT_PIN, LOW); break;
          case 'U': digitalWrite(REAR_LIGHT_PIN, HIGH); break;
          case 'u': digitalWrite(REAR_LIGHT_PIN, LOW); break;

          // Horn (New App Protocol)
          case 'H': digitalWrite(HORN_PIN, HIGH); break;
          case 'h': digitalWrite(HORN_PIN, LOW); break;
          
          // Horn (Old Protocol Compatibility)
          case 'V': digitalWrite(HORN_PIN, HIGH); break;
          case 'v': digitalWrite(HORN_PIN, LOW); break;
        }
      }
    }
  }
}

//====================================================//
//                 MOVEMENT FUNCTIONS                 //
//====================================================//
void leftForward(int speed) {
  analogWrite(LEFT_LPWM, 0);
  analogWrite(LEFT_RPWM, speed);
}

void leftBackward(int speed) {
  analogWrite(LEFT_RPWM, 0);
  analogWrite(LEFT_LPWM, speed);
}

void rightForward(int speed) {
  analogWrite(RIGHT_LPWM, 0);
  analogWrite(RIGHT_RPWM, speed);
}

void rightBackward(int speed) {
  analogWrite(RIGHT_RPWM, 0);
  analogWrite(RIGHT_LPWM, speed);
}

void forward() {
  leftForward(motorSpeed);
  rightForward(motorSpeed);
}

void backward() {
  leftBackward(motorSpeed);
  rightBackward(motorSpeed);
}

void left() {
  leftBackward(turnSpeed);
  rightForward(turnSpeed);
}

void right() {
  leftForward(turnSpeed);
  rightBackward(turnSpeed);
}

void forwardLeft() {
  leftForward(diagonalSpeed);
  rightForward(motorSpeed);
}

void forwardRight() {
  leftForward(motorSpeed);
  rightForward(diagonalSpeed);
}

void backLeft() {
  leftBackward(diagonalSpeed);
  rightBackward(motorSpeed);
}

void backRight() {
  leftBackward(motorSpeed);
  rightBackward(diagonalSpeed);
}

void stopRobot() {
  analogWrite(LEFT_RPWM, 0);
  analogWrite(LEFT_LPWM, 0);
  analogWrite(RIGHT_RPWM, 0);
  analogWrite(RIGHT_LPWM, 0);
}

//====================================================//
//               SMOOTH ARM MOVEMENT                  //
//====================================================//
void smoothMove(Servo &servo, int target, int &currentPos) {
  target = max(0, min(180, target)); // Clamp to safe limits
  
  if (currentPos < target) {
    for (int i = currentPos; i <= target; i++) {
      servo.write(i);
      delay(MOVE_DELAY);
    }
  } 
  else if (currentPos > target) {
    for (int i = currentPos; i >= target; i--) {
      servo.write(i);
      delay(MOVE_DELAY);
    }
  }
  currentPos = target;
}
