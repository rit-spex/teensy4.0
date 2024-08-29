#include <Arduino.h>

#include "../include/Constants.h"

// The drivebase is the main body board
#include "../include/MainBodyBoard.h"

// These allow the teensy to be the main controller
#if MASTER_TEENSY

// includes for the xbee
#if ENABLE_XBEE
#include "../include/Xbee.h"
#endif

// includes for the arm
#if ENABLE_ARM
#include <Wire.h>
#include "../include/Arm.h"
#endif
#endif

// create the main body board
MainBodyBoard mbb;

#if MASTER_TEENSY
#if ENABLE_XBEE
// create the xbee
Xbee xbee;
#endif

#if ENABLE_ARM
// create the arm
Arm arm;
#endif
#endif

void setup() 
{
  // start up the main body board, this will turn the status light off
  mbb.startUp();

  #if ENABLE_SERIAL
  //this is the connection to the computer
  Serial.begin(9600);
  Serial.println("Main Body Board");
  #endif

  #if MASTER_TEENSY
  //this is the connection to the xbee
  #if ENABLE_XBEE
  //xbee = Xbee();  
  Serial2.begin(9600, SERIAL_8N1);  
  #endif

  delay(5000);

  #if ENABLE_ARM
  arm.startUp();

  // start everything that is needed for arm
  Wire.begin();
  Wire.setSCL(19);
  Wire.setSDA(18);

  // this needs to be moved to the arm class
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
  #endif

  // time to allow the arm to send its commands
  delay(5000);
  #endif

  // need to change this
  digitalWrite(STATUS_LIGHT_PIN, LOW);
}

unsigned long lastTime = millis();

void loop() 
{ 
  if(millis() - UPDATE_RATE_MS > lastTime)
  {
    #if MASTER_TEENSY
    #if ENABLE_XBEE
    xbee.UpdateValues();
    if(xbee.isNewValues() && xbee.isActive())
    { 
      #if ENABLE_ARM
      //move the claw, make sure that both buttons are not pressed
      if(xbee.getCurrentValue(Xbee::CONTROLLER::A_BUTTON) && !xbee.getCurrentValue(Xbee::CONTROLLER::B_BUTTON))
      {
        arm.moveClaw(Arm::Direction::FORWARD);
      } 
      else if(xbee.getCurrentValue(Xbee::CONTROLLER::B_BUTTON) && !xbee.getCurrentValue(Xbee::CONTROLLER::A_BUTTON))
      {
        arm.moveClaw(Arm::Direction::REVERSE);
      }
      else
      {
        arm.moveClaw(Arm::Direction::OFF);
      }

      //move the wrist, make sure that both buttons are not pressed
      if(xbee.getCurrentValue(Xbee::CONTROLLER::X_BUTTON) && !xbee.getCurrentValue(Xbee::CONTROLLER::Y_BUTTON))
      {
        arm.moveWrist(Arm::Direction::FORWARD);
      } 
      else if(xbee.getCurrentValue(Xbee::CONTROLLER::Y_BUTTON) && !xbee.getCurrentValue(Xbee::CONTROLLER::X_BUTTON))
      {
        arm.moveWrist(Arm::Direction::REVERSE);
      }
      else
      {
        arm.moveWrist(Arm::Direction::OFF);
      }

      //move the shoulder, make sure that both buttons are not pressed
      if(xbee.getCurrentValue(Xbee::CONTROLLER::LB_BUTTON) && !xbee.getCurrentValue(Xbee::CONTROLLER::RB_BUTTON))
      {
        arm.moveShoulder(Arm::Direction::FORWARD);
      } 
      else if(xbee.getCurrentValue(Xbee::CONTROLLER::RB_BUTTON) && !xbee.getCurrentValue(Xbee::CONTROLLER::LB_BUTTON))
      {
        arm.moveShoulder(Arm::Direction::REVERSE);
      }
      else
      {
        arm.moveShoulder(Arm::Direction::OFF);
      }

      //move the base, make sure that both buttons are not pressed
      if(xbee.getCurrentValue(Xbee::CONTROLLER::LEFT_TRIGGER) && !xbee.getCurrentValue(Xbee::CONTROLLER::RIGHT_TRIGGER))
      {
        arm.moveBase(Arm::Direction::FORWARD);
      } 
      else if(xbee.getCurrentValue(Xbee::CONTROLLER::RIGHT_TRIGGER) && !xbee.getCurrentValue(Xbee::CONTROLLER::LEFT_TRIGGER))
      {
        arm.moveBase(Arm::Direction::REVERSE);
      }
      else
      {
        arm.moveBase(Arm::Direction::OFF);
      }
      #endif
    }
    

    #if ENABLE_DRIVEBASE
    mbb.drive(-xbee.getCurrentValue(Xbee::CONTROLLER::LEFT_Y_AXIS), xbee.getCurrentValue(Xbee::CONTROLLER::RIGHT_Y_AXIS));
    #endif

    #endif
    #endif
    mbb.updateSubsystems(millis() - lastTime);
    lastTime = millis();
  }
}