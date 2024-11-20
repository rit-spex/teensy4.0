#include "../include/Constants.h"
#include "../include/DEBUG.h"

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

#if ENABLE_DEMO_ENCODER
// #define ENCODER_RUN_CYCLE_MICROSEC 1000
// IntervalTimer encoderTimer;
QuadratureDecoder demo_encoder{ENC_A_PIN_0, ENC_B_PIN_0};
#endif

unsigned long currentRunCycle = 0;

// create the main body board
MainBodyBoard mbb;

void setup()
{
  // start up the main body board, this will turn the status light off
  mbb.startUp();

  #if ENABLE_SERIAL
  //this is the connection to the computer
  Serial.begin(9600);
  Serial.println("Main Body Board");
  #endif

  #if ENABLE_DEMO_ENCODER
  demo_encoder.begin();
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
    
    // subtract the current cycle by one to get the entire timeframe
    mbb.updateSubsystems(millis() - UPDATE_RATE_MS * (currentRunCycle-1));

    // the increment to the next cycle
    currentRunCycle++;
  }
}
