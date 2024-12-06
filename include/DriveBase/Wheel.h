/*
File: wheel.h
Author: Ryan Barry
Date Created: 1/23/2024

This file defines the wheel class for the rover.

This class is responsible for controlling each individual wheel of the rover
based on its target speed.
*/

#ifndef WHEEL_H
#define WHEEL_H

#include "Pinout.h"
#include "Constants.h"
#include "DEBUG.h"

#include "Motor.h"
#include "QuadDecoder.h"
#include "PIDController.h"

class Wheel {
    public:
        /*
        * Constructor for the wheel class
        * @param wheel_id: which wheel is the current one
        */
        Wheel(uint8_t wheel_id);

        /*
        * Adjust's the PWM signal to the wheel to match the target speed
        * @param targetSpeed The target speed of the wheel
        */
        void setSpeed(float targetSpeed);


        #if ENABLE_ENCODER
        void setRPM(float targetRPM);

        /*
        * Gets the current RPM of the wheel
        * @return The current RPM of the wheel
        */
        float getRPM();

        /*
        * Updates the PID controller for the wheel
        */
        void updatePID(int timeInterval_ms);
        #endif

        void forceStop();

    private:

        Motor m_motor;

        #if ENABLE_ENCODER
        QuadratureDecoder m_encoder;
        PIDController m_pid;

        float m_targetRPM;
        float m_currentRPM;
        #endif
};

#endif
