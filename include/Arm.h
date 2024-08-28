#ifndef ARM_H
#define ARM_H
#include <Arduino.h>
#include <TimerThree.h>
#include <Wire.h>
#include <Tic.h>

// define these pins, wristDir and shoulderDir can be any digital output
// wristSpeed and shoulderSpeed need to be pins 9 and 6
#define WRIST_DIR_PIN 33
#define WRIST_SPEED_PIN 6 // MUST BE PIN 6
#define SHOULDER_DIR_PIN 34
#define SHOULDER_SPEED_PIN 9 // MUST BE PIN 9

// I2C device number
#define BASE_I2C_ID 15 // brushed motor
#define BASE_MAX_SPEED 800
#define CLAW_I2C_ID 16 // stepper motor
#define CLAW_MAX_SPEED (200 * 10000) // 100 steps per second

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
        
        TicI2C tic{CLAW_I2C_ID}; // stepper motor

    private:

        // Changing this time will change the motor speeds 30 us seems to be a good starting speed
        int time = 80; // time in microseconds <- lower num mean faster fun
};

#endif