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

// if the teensy is the main brain
#define MASTER_TEENSY    true

// enable each subsystem
#define ENABLE_CAN       false
#define ENABLE_DRIVEBASE true
#define ENABLE_TEMP      false
#define ENABLE_XBEE      true
#define ENABLE_SERIAL    false
#define ENABLE_ARM       false