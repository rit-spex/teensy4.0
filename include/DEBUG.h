/*
File: DEBUG.h
Author: Tyler Halifax
Date Created: 3/21/2024

This file is used to define the debug flags for the rover code.
If disable is defined, the corresponding subsystem will be disabled.
If debug is defined, the corresponding subsystem will be serial printed
*/

// enable when encoders are on the base
#define ENABLE_ENCODER   false

// enable to test with a single encoder
#define ENABLE_DEMO_ENCODER false

// if the teensy is the main brain - this will enable both xbee and arm
#define MASTER_TEENSY    false

// this will disable all hardware and simulate the rover. DO NOT ENABLE ON THE ROVER
#define ENABLE_SIMULATOR false

// enable each subsystem
#define ENABLE_CAN       true
#define ENABLE_DRIVEBASE true
#define ENABLE_TEMP      false
#define ENABLE_SERIAL    true