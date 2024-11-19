/*
File: drive_base.h
Author: Ryan Barry
Date Created: 1/23/2024

This file defines the drive base class for the rover.

This class is responsible for controlling the rover's wheels based on 
the target velocity transmitted over CAN.
*/

#ifndef DRIVE_BASE_H
#define DRIVE_BASE_H

#include "./Constants.h"
#include "./DEBUG.h"
#include "./Pinout.h"
#include <math.h>

#include "Wheel.h"

#ifdef ENABLE_CAN
#include "CAN.h"
#endif

class DriveBase {
    public:
        #if ENABLE_CAN
            DriveBase(CAN *can);
        #else
            DriveBase();
        #endif

        #if ENABLE_ENCODER
        // Updates the RPM of the rover's wheels. motor final speed is calculated by the PID controller
        void updateRPM(int timeInterval_ms);
        #else
        // Updates the speed of the rover's wheel. Speed is percent
        void updateSingleWheelSpeed(int wheelIndex, float targetSpeed);
        #endif
        void drive(float left_axis, float right_axis);

        // stop the wheels immedately
        void forceStop();
    private:
        // An array of the rover's wheels
        Wheel m_wheels[NUM_WHEELS];

        // CAN object for the rover
        #if ENABLE_CAN
        CAN *m_CAN;
        #endif

        #if ENABLE_ENCODER
        
        // An array of the target RPMs corresponding to each wheel
        float m_targetRPM[NUM_WHEELS];

        #if ENABLE_CAN
        void getTargetRPM();
        #endif
        #endif
};
#endif