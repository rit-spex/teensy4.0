#ifndef ARM_H
#define ARM_H

#include "DEBUG.h"
#include "Pinout.h"
#include "Constants.h"

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#include <TimerThree.h>
#include <Wire.h>
#include <Tic.h>
#endif

class Arm
{
    public:
        enum Direction
        {
            FORWARD = 1,
            REVERSE = 0,
            OFF = 2
        };
        Arm();
        ~Arm();
        void startUp();
        // functions to move the harmonic drives. These are the exact same but with different pin outputs
        // Functions work by setting timer output at either 50% duty cycle
        void moveShoulder(Direction direction);
        void moveWrist(Direction direction);
        void moveBase(Direction direction);
        void moveClaw(Direction direction);
        void moveArm(Direction shoulderDirection, Direction wristDirection, Direction baseDirection, Direction clawDirection);
        
        void disable();

        // stepper motor
        TicI2C tic{CLAW_I2C_ID}; 
    private:

        // if disable flag is true then stop all arm motion and prevent the arm from moving
        bool m_disabled = false;

        // Changing this time will change the motor speeds 30 us seems to be a good starting speed
        int time = 80; // time in microseconds <- lower num mean faster fun
};

#endif