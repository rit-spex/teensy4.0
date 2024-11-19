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

#include "./Pinout.h"
#include "./Constants.h"
#include "./DEBUG.h"

#include "Motor.h"
#include "QuadDecoder.h"
#include "PIDController.h"

class Wheel {
    public:
        #if ENABLE_ENCODER
        /*
        * Constructor for the wheel class
        * @param pwm_pin The PWM pin the wheel is connected to
        * @param enc_A_pin The A pin of the wheel's quadrature encoder
        * @param enc_B_pin The B pin of the wheel's quadrature encoder
        */
        Wheel(PWM_PINS pwm_pin, int direction, ENC_A_PINS enc_A_pin, ENC_B_PINS enc_B_pin, double kp, double ki, double kd);
        #else
        /*
        * Constructor for the wheel class if there is no encoder
        */
        Wheel(PWM_PINS pwm_pin, int direction);
        #endif

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
