/*
File: main_body_board.h
Author: Ryan Barry
Date Created: 1/23/2024

This file defines the main body board class for the rover.

The main body board is responsible for:
- controlling the drive wheels with encoder feedback
- reading the temperature of the thermistors
*/

#ifndef MAIN_BODY_BOARD_H
#define MAIN_BODY_BOARD_H

#include "Constants.h"
#include "DEBUG.h"
#include "Pinout.h"
#include <math.h>

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#endif

// All of the subsystems
#if ENABLE_DRIVEBASE
#include "DriveBase/DriveBase.h"
#endif

#if ENABLE_TEMP
#include "Temperature/TemperatureSubsystem.h"
#endif

#if ENABLE_CAN
#include "CAN.h"
#endif

class MainBodyBoard {
    public:
        /*
        * Constructor for the main body board class.
        * Initializes the drive base, temp subsystem, and LiDAR.
        */
        MainBodyBoard();
        ~MainBodyBoard();

        // startup for all of the subsystems
        void startUp();

        // increments a time then will blink the status light
        void BlinkStatusLight();

        // updates all of the subsystems
        void updateSubsystems(int timeInterval_ms);

        // run any background process while it is not doing main tasks
        void runBackgroundProcess(void);

        // disables the teensy
        void disable();

        // check if the mbb is disabled
        bool isDisabled();

        // Drives the rover based on the left and right joystick values - ONLY FOR MASTER_TEENSY
        #if ENABLE_DRIVEBASE
        #if MASTER_TEENSY
        void drive(float left_axis, float right_axis);
        #endif
        #endif
    private:

        bool m_statusLightOn = false;
        int  m_statusLightWait = 0;
        bool m_disabled = false;

        #if ENABLE_CAN
            CAN m_can = CAN();
        #endif

        #if ENABLE_DRIVEBASE
            #if ENABLE_CAN
                DriveBase m_drive_base = DriveBase(&m_can);
            #else
                DriveBase m_drive_base = DriveBase();
            #endif
        #endif

        #if ENABLE_TEMP
            #if ENABLE_CAN
                TemperatureSubsystem m_temperature_subsystem = TemperatureSubsystem(&m_can);
            #else
                TemperatureSubsystem m_temperature_subsystem = TemperatureSubsystem();
            #endif
        #endif
};

#endif
