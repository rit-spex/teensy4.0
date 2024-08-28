#include <Arduino.h>
#include "../include/MainBodyBoard.h"
#include "../include/Motor.h"
#include "../include/Xbee.h"
#include "../include/Arm.h"
#include <Servo.h>
#include <Wire.h>

// unicast mode
// Configure the DL/DH of the xbee to the SL/SH of the xbee talk directly to each other
// Configure the RR (MAC retries to 0) - doesn't wait for an ACK
// RO = 0; packetization timeout to 0 - doesn't wait for an
// NP - Maximum number of bytes;

// xbee recomend 10ms delay between packets

//int armMotor = 15;

MainBodyBoard mbb;
Arm arm;
Xbee xbee;
// Servo motor1;
// Servo motor2;
// Servo motor3;
// Servo motor4;
// Servo motor5;
// Servo motor6;


// void static exitSafeStart(int motor)
// {
//   Wire.beginTransmission(motor);
//   Wire.write(0x83);
//   Wire.endTransmission();
// }

// void static setMotorSpeed(int16_t speed, int motor)
// {
//   uint8_t cmd = 0x85;
//   if(speed <0)
//   {
//     cmd = 0x86;
//     speed = -speed;
//   }
//   Wire.beginTransmission(motor);
//   Wire.write(cmd);
//   Wire.write(speed & 0x1F);
//   Wire.write(speed >> 5 & 0x7F);
//   Wire.endTransmission();
// }

void setup() 
{
  pinMode(STATUS_LIGHT_PIN, OUTPUT);
  digitalWrite(STATUS_LIGHT_PIN, HIGH);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1);  
  
  xbee = Xbee();

  delay(5000);

  arm.startUp();

  // start everything that is needed for arm
  Wire.begin();
  Wire.setSCL(19);
  Wire.setSDA(18);

  delay(100);
  Wire.beginTransmission(BASE_I2C_ID);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
  delay(20);

  // Initialize the Tic object
  arm.tic.exitSafeStart();

  arm.moveShoulder(Arm::Direction::OFF);
  arm.moveWrist(Arm::Direction::OFF);
  arm.moveBase(Arm::Direction::OFF);
  arm.moveClaw(Arm::Direction::OFF);

  Serial.println("Main Body Board");

  delay(5000);
  digitalWrite(STATUS_LIGHT_PIN, LOW);
  //analogWriteFrequency(PWM_PIN_0, 100);
  // motor1.attach(PWM_PIN_0, 1400, 1600);
  // motor2.attach(PWM_PIN_1, 1400, 1600);
  // motor3.attach(PWM_PIN_2, 1400, 1600);
  // motor4.attach(PWM_PIN_3, 1400, 1600);
  // motor5.attach(PWM_PIN_4, 1400, 1600);
  // motor6.attach(PWM_PIN_5, 1400, 1600);

  //motor.write(100);
  // motor1.writeMicroseconds(1500);
  // motor2.writeMicroseconds(1500);
  // motor3.writeMicroseconds(1500);
  // motor4.writeMicroseconds(1500);
  // motor5.writeMicroseconds(1500);
  // motor6.writeMicroseconds(1500);
  // motor.setSpeed(100);
}

float rightAxis = 0.0;
float leftAxis = 0.0;
bool aButton = false;
bool bButton = false;
bool xButton = false;
bool yButton = false;
bool lbButton = false;
bool rbButton = false;
bool ltButton = false;
bool rtButton = false;

bool newValues = false;

void loop() 
{
  xbee.UpdateValues();
  float newleftAxis = xbee.getCurrentValue(Xbee::CONTROLLER::LEFT_Y_AXIS);
  if(newleftAxis != leftAxis)
  {
    newValues = true;
    leftAxis = newleftAxis;
  }
  float newrightAxis = xbee.getCurrentValue(Xbee::CONTROLLER::RIGHT_Y_AXIS);
  if(newrightAxis != rightAxis)
  {
    newValues = true;
    rightAxis = newrightAxis;
  }
  bool newAButton = xbee.getCurrentValue(Xbee::CONTROLLER::A_BUTTON);
  if(newAButton != aButton)
  {
    newValues = true;
    aButton = newAButton;
  }
  bool newBButton = xbee.getCurrentValue(Xbee::CONTROLLER::B_BUTTON);
  if(newBButton != bButton)
  {
    newValues = true;
    bButton = newBButton;
  }
  bool newXButton = xbee.getCurrentValue(Xbee::CONTROLLER::X_BUTTON);
  if(newXButton != xButton)
  {
    newValues = true;
    xButton = newXButton;
  }
  bool newYButton = xbee.getCurrentValue(Xbee::CONTROLLER::Y_BUTTON);
  if(newYButton != yButton)
  {
    newValues = true;
    yButton = newYButton;
  }
  bool newLBButton = xbee.getCurrentValue(Xbee::CONTROLLER::LB_BUTTON);
  if(newLBButton != lbButton)
  {
    newValues = true;
    lbButton = newLBButton;
  }
  bool newRBButton = xbee.getCurrentValue(Xbee::CONTROLLER::RB_BUTTON);
  if(newRBButton != rbButton)
  {
    newValues = true;
    rbButton = newRBButton;
  }
  bool newLTButton = xbee.getCurrentValue(Xbee::CONTROLLER::LEFT_TRIGGER);
  if(newLTButton != ltButton)
  {
    newValues = true;
    ltButton = newLTButton;
  }
  bool newRTButton = xbee.getCurrentValue(Xbee::CONTROLLER::RIGHT_TRIGGER);
  if(newRTButton != rtButton)
  {
    newValues = true;
    rtButton = newRTButton;
  }
  if(newValues)
  {
    Serial.print("Left Axis: ");
    Serial.print(leftAxis);
    Serial.print(" Right Axis: ");
    Serial.print(rightAxis);
    Serial.print(" A Button: ");
    Serial.print(aButton);
    Serial.print(" B Button: ");
    Serial.print(bButton);
    Serial.print(" X Button: ");
    Serial.print(xButton);
    Serial.print(" Y Button: ");
    Serial.print(yButton);
    Serial.print(" LB Button: ");
    Serial.print(lbButton);
    Serial.print(" RB Button: ");
    Serial.print(rbButton);
    Serial.print(" LT Button: ");
    Serial.print(ltButton);
    Serial.print(" RT Button: ");
    Serial.println(rtButton);

    mbb.drive(-leftAxis, rightAxis);

    if(aButton && !bButton)
    {
      arm.moveClaw(Arm::Direction::FORWARD);
    } 
    else if(bButton && !aButton)
    {
      arm.moveClaw(Arm::Direction::REVERSE);
    }
    else
    {
      arm.moveClaw(Arm::Direction::OFF);
    }

    if(xButton && !yButton)
    {
      arm.moveWrist(Arm::Direction::FORWARD);
    } 
    else if(yButton && !xButton)
    {
      arm.moveWrist(Arm::Direction::REVERSE);
    }
    else
    {
      arm.moveWrist(Arm::Direction::OFF);
    }

    if(lbButton && !rbButton)
    {
      arm.moveShoulder(Arm::Direction::FORWARD);
    } 
    else if(rbButton && !lbButton)
    {
      arm.moveShoulder(Arm::Direction::REVERSE);
    }
    else
    {
      arm.moveShoulder(Arm::Direction::OFF);
    }

    if(ltButton && !rtButton)
    {
      arm.moveBase(Arm::Direction::FORWARD);
    } 
    else if(rtButton && !ltButton)
    {
      arm.moveBase(Arm::Direction::REVERSE);
    }
    else
    {
      arm.moveBase(Arm::Direction::OFF);
    }

    // motor1.writeMicroseconds(1500 + (leftAxis * 100));
    // motor2.writeMicroseconds(1500 + (leftAxis * 100));
    // motor3.writeMicroseconds(1500 + (leftAxis * 100));
    // motor4.writeMicroseconds(1500 - (rightAxis * 100));
    // motor5.writeMicroseconds(1500 - (rightAxis * 100));
    // motor6.writeMicroseconds(1500 - (rightAxis * 100));

    newValues = false;
  }
    //mbb.drive(-leftAxis, rightAxis);
  // Serial.print("error count: ");
  // Serial.print(xbee.error_count);
  // Serial.print(" good count: ");
  // Serial.println(xbee.good_count);
  //arm.moveElbow(Arm::Direction::FORWARD);
  delay(40); // 40 is good
  //Serial.println("Shoulder Forward");
  //motor.setSpeed(1);
  //motor.updateSingleWheel(1, 1);
  // motor1.writeMicroseconds(1000);
  // motor2.writeMicroseconds(1000);
  // motor3.writeMicroseconds(1000);
  // motor4.writeMicroseconds(1000);
  // motor5.writeMicroseconds(1000);
  // motor6.writeMicroseconds(1000);
  //xbee.UpdateValues();
  //String result = "";
  //for(int Controller = 0; Controller < 10; Controller++)
  //{
  //  result += (String(Controller) + ": " + String(xbee.getCurrentValue((Xbee::CONTROLLER)Controller)) + " ");
  //}
  //Serial.println(result);
  //motor.writeMicroseconds(1700);
  // if(lastcounter != Serial2.available())
  // {
  //   gotSignal = true;
  //   counter = 0;
  //   lastcounter = Serial2.available();
  //   while(Serial2.available() > 3)
  // {
  //   //Serial.println("Serial2 is available");
  //   int input = Serial2.read();
  //   if(input == 0xDE)
  //   {
  //     float joy1 = Serial2.read();
  //     float joy2 = Serial2.read();
  //     if(joy1 > 200)
  //     {
  //       Serial.println("Ignore");
  //     }
  //     else
  //     {
  //       Serial.print("joy1 ");
  //       //Serial.print(joy1, DEC);
  //       leftAxis = (joy1 - 100.0) / 100.0;
  //       Serial.println(leftAxis);
  //       // motor1.writeMicroseconds(1500 + (joy1 - 100));
  //       // motor2.writeMicroseconds(1500 + (joy1 - 100));
  //       // motor3.writeMicroseconds(1500 + (joy1 - 100));
  //     }
  //     if(joy2 > 200)
  //     {
  //       Serial.println("Ignore");
  //     }
  //     else
  //     {
  //       Serial.print("joy2 ");
  //       //Serial.print(joy2, DEC);
  //       rightAxis = (joy2 - 100.0) / 100.0;
  //       Serial.println(rightAxis);
  //       // motor4.writeMicroseconds(1500 - (joy2 + 100));
  //       // motor5.writeMicroseconds(1500 - (joy2 + 100));
  //       // motor6.writeMicroseconds(1500 - (joy2 + 100));
  //     }
  //   }
  //   mbb.drive(-leftAxis, rightAxis);
  // }
    //mbb.updateSubsystems();
    //input += (char)Serial2.read();
    //Serial.println( (char)Serial2.read());
    //if(input == 106 || input == 250)
    //{
    //  int increase = Serial2.read();
    //  motor.writeMicroseconds(1500 + (increase - 128));
    //  Serial.println("Motor speed set to: " + String(motor.readMicroseconds()));
    //}
    //
    //digitalWrite(STATUS_LIGHT_PIN, LOW);    
    //if(statusLightOn)
    //{
    //  digitalWrite(STATUS_LIGHT_PIN, LOW);
    //  statusLightOn = false;
    //}
    //else
    //{
    //  digitalWrite(STATUS_LIGHT_PIN, HIGH);
    //  statusLightOn = true;
    //}
    
  // }
  // else if(gotSignal)
  // {
  //   counter++;
  // }
  
  // if(counter > 30)
  // {
    //motor1.writeMicroseconds(1500);
    //motor2.writeMicroseconds(1500);
    //motor3.writeMicroseconds(1500);
    //motor4.writeMicroseconds(1500);
    //motor5.writeMicroseconds(1500);
    //motor6.writeMicroseconds(1500);
  //   mbb.drive(0.0, 0.0);
  //   digitalWrite(STATUS_LIGHT_PIN, HIGH);    

  //   while (true)
  //   {
  //     /* code */
  //   } 
  // }
  
  // //setMotorSpeed(400,15);
  // delay(100);

}