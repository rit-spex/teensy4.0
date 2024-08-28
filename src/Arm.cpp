#include "../include/Arm.h"

Arm::Arm()
{
    // Set used pins to output
    pinMode(WRIST_DIR_PIN, OUTPUT);
    pinMode(WRIST_SPEED_PIN, OUTPUT);
    pinMode(SHOULDER_DIR_PIN, OUTPUT);
    pinMode(SHOULDER_SPEED_PIN, OUTPUT);  

    // Initialize and start timer to outut correct PWM signals
    Timer3.initialize(time);
    Timer3.start();
}

Arm::~Arm()
{
  // Stop timer
  Timer3.stop();
}

void Arm::startUp()
{
  Timer3.stop();
  Timer3.initialize(time);
  Timer3.start();
}

void Arm::moveShoulder(Direction direction)
{
  Serial.println("Moving shoulder");
  // If direction is not OFF, move motor
  if(direction != OFF)
  {
    // Write direction, HIGH is one way LOW is the other
    digitalWrite(SHOULDER_DIR_PIN, (int)direction);
    Timer3.pwm(SHOULDER_SPEED_PIN, 511);
  }
  // If direction is OFF, stop motor
  else if(direction == OFF)
  {
    Timer3.pwm(SHOULDER_SPEED_PIN, 0);
  }
}

void Arm::moveWrist(Direction direction)
{
  Serial.println("Moving wrist");
  // If direction is not OFF, move motor
  if(direction != OFF)
  {
    // Write direction, HIGH is one way LOW is the other
    digitalWrite(WRIST_DIR_PIN, (int)direction);
    Timer3.pwm(WRIST_SPEED_PIN, 511);
  }
  // If direction is OFF, stop motor
  else if(direction == OFF)
  {
    Timer3.pwm(WRIST_SPEED_PIN, 0);
  }
}

void Arm::moveBase(Direction direction)
{
  Serial.println("Moving base");
  uint8_t cmd = 0x85;
  int speed = 0;
  if(direction == FORWARD)
  {
    cmd = 0x85;  // Motor forward
    speed = BASE_MAX_SPEED; // needs to be positive
  }
  else if(direction == REVERSE)
  {
    cmd = 0x86;  // Motor reverse
    speed = BASE_MAX_SPEED; // needs to be positive
  }
  else if(direction == OFF)
  {
    cmd = 0x85;  // Can be either direction to stop motor
    speed = 0; // stop motor
  }
  Wire.beginTransmission(BASE_I2C_ID);
  Wire.write(cmd);
  Wire.write(speed & 0x1F);
  Wire.write(speed >> 5 & 0x7F);
  Wire.endTransmission();
}

void Arm::moveClaw(Direction direction)
{
  Serial.println("Moving claw");
  if(direction == FORWARD)
  {
    tic.setTargetVelocity(CLAW_MAX_SPEED);
  }
  else if(direction == REVERSE)
  {
    tic.setTargetVelocity(-CLAW_MAX_SPEED);
  }
  else if(direction == OFF)
  {
    tic.haltAndHold();
  }
}

void Arm::moveArm(Direction shoulderDirection, Direction wristDirection, Direction elbowDirection, Direction clawDirection)
{
  moveShoulder(shoulderDirection);
  moveWrist(wristDirection);
  moveBase(elbowDirection);
  moveClaw(clawDirection);
}