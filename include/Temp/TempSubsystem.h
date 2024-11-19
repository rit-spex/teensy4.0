/*
File: temp_subsystem.h
Author: Ryan Barry
Date Created: 1/23/2024

This file defines the temp subsystem class for the rover.

This class is responsible for reading the temperature of the thermistors.
*/

#ifndef TEMP_SUBSYSTEM_H
#define TEMP_SUBSYSTEM_H

#include "./Constants.h"
#include "./Pinout.h"
#include "./DEBUG.h"

#include "Fan.h"
#include "Thermistor.h"

#if ENABLE_CAN
#include "CAN.h"
#endif

using namespace std;

class TempSubsystem {
    public:
        #if ENABLE_CAN
        TempSubsystem(CAN *can);
        #else
        TempSubsystem();
        #endif

        float* getTemperature();
        void setFansPower(int power);
        /*
         * Update the temperature and fan power
        */
        void updateFans(void);
    private:
        // Array of Thermistor objects
        Thermistor m_thermistors[NUM_THERMISTORS];

        // Array of Fan objects
        Fan m_fans[NUM_FANS];

        // Array of temperature readings
        float m_temperature[NUM_THERMISTORS];

        #if ENABLE_CAN
        // Pointer to the CAN object
        CAN *m_can;
        #endif
};

#endif