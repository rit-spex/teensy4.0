#include "../include/Constants.h"
#include "../include/DEBUG.h"
#include "../include/Pinout.h"

// the simulator will emulate the hardware
#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#endif

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
#if ENABLE_SIMULATOR
#else
#include <Wire.h>
#endif // ENABLE_SIMULATOR
#include "../include/Arm.h"
#endif
#endif

unsigned long currentRunCycle = 0;

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
  Serial2.begin(9600, SERIAL_8N1);
  #endif

  delay(5000);

  #if ENABLE_ARM
  arm.startUp();

  // start everything that is needed for arm
  Wire.begin();
  Wire.setSCL(ARM_SDA_PIN);
  Wire.setSDA(ARM_SCL_PIN);

  // this needs to be moved to the arm class
  delay(5000);
  Wire.beginTransmission(BASE_I2C_ID);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
  delay(5000);

  // Initialize the Tic object
  arm.tic.exitSafeStart();

  arm.moveShoulder(Arm::Direction::OFF);
  arm.moveWrist(Arm::Direction::OFF);
  arm.moveBase(Arm::Direction::OFF);
  arm.moveClaw(Arm::Direction::OFF);
  #endif

  // time to allow the arm to send its commands
  delay(500);
  #endif

  //update the currentRunCycle to be synced with the current time
  currentRunCycle = floor(millis()/UPDATE_RATE_MS);
}

void loop()
{
  if(millis() >= UPDATE_RATE_MS * currentRunCycle)
  {
    #if ENABLE_SERIAL
      Serial.print("current cycle: ");
      Serial.println((int)currentRunCycle);
    #endif // ENABLE_SERIAL

    #if MASTER_TEENSY
    #if ENABLE_XBEE
    xbee.UpdateValues();
    if(xbee.isNewValues() && xbee.isActive())
    {
      xbee.printValues();
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

    if(xbee.isActive() && !xbee.isNewValuesFound)
    {
      mbb.disable();
    }

    #if ENABLE_DRIVEBASE
    mbb.drive(xbee.getCurrentValue(Xbee::CONTROLLER::LEFT_Y_AXIS), xbee.getCurrentValue(Xbee::CONTROLLER::RIGHT_Y_AXIS));
    #endif

    #endif
    #endif
    // subtract the current cycle by one to get the entire timeframe
    mbb.updateSubsystems(millis() - UPDATE_RATE_MS * (currentRunCycle-1));

    // the increment to the next cycle
    currentRunCycle++;
  }
}
