#include "../include/Constants.h"
#include "../include/DEBUG.h"
#include "../include/Pinout.h"

#if ENABLE_DEMO_ENCODER
#include "../include/QuadDecoder.h"
#endif

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
#include "../include/Extra/Xbee.h"
#endif

// includes for the arm
#if ENABLE_ARM
#if ENABLE_SIMULATOR
#else
#include <Wire.h>
#endif // ENABLE_SIMULATOR
#include "../include/Extra/Arm.h"
#endif
#endif

  #if ENABLE_DEMO_ENCODER
  // #define ENCODER_RUN_CYCLE_MICROSEC 1000
  // IntervalTimer encoderTimer;
  QuadratureDecoder demo_encoder{ENC_A_PIN_0, ENC_B_PIN_0};
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

// #if ENABLE_DEMO_ENCODER
// void TEST()
// {
//   demo_encoder.updateCount(1);
// }
// #endif

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

  delay(1000);

  #if ENABLE_ARM
  arm.startUp();

  // start everything that is needed for arm
  Wire.begin();
  Wire.setSCL(ARM_SDA_PIN);
  Wire.setSDA(ARM_SCL_PIN);

  // this needs to be moved to the arm class
  delay(1000);
  Wire.beginTransmission(BASE_I2C_ID);
  Wire.write(0x83);  // Exit safe start
  Wire.endTransmission();
  delay(1000);

  // Initialize the Tic object
  arm.tic.exitSafeStart();

  arm.moveShoulder(Arm::Direction::OFF);
  arm.moveWrist(Arm::Direction::OFF);
  arm.moveBase(Arm::Direction::OFF);
  arm.moveClaw(Arm::Direction::OFF);
  #endif

  // time to allow the arm to send its commands
  delay(1000);
  #endif

  #if ENABLE_DEMO_ENCODER
  demo_encoder.begin();
  // encoderTimer.begin(TEST, 1000);
  #endif

  //update the currentRunCycle to be synced with the current time
  currentRunCycle = floor(millis()/UPDATE_RATE_MS);
}

void loop()
{
  mbb.runBackgroundProcess();
  if(millis() >= UPDATE_RATE_MS * currentRunCycle)
  {
    #if ENABLE_SERIAL
      Serial.print("current cycle: ");
      Serial.println((int)currentRunCycle);

      #if ENABLE_DEMO_ENCODER
      Serial.println(demo_encoder.getRPM(millis() - UPDATE_RATE_MS * (currentRunCycle-1)));
      #endif
    #endif // ENABLE_SERIAL
    
    #if MASTER_TEENSY
    #if ENABLE_XBEE
    xbee.UpdateValues();

    // check if xbee is disabled
    if(xbee.isDisabled())
    {
      mbb.disable();
    }

    // if mainbodyboard is disabled then disable everything
    if (mbb.isDisabled())
    {
      #if ENABLE_ARM
      arm.disable();
      #endif
    }

    if(xbee.isNewValues() && !xbee.isDisabled())
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
