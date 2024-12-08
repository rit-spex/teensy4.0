#ifndef MOTOR_H
#define MOTOR_H

#include "Pinout.h"
#include "Constants.h"
#include "DEBUG.h"
#include <math.h>

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#include <Servo.h>
#endif

// Motor class for controlling the motors, this is meant to be a generic class for all motors
class Motor {
    public:
        Motor(uint8_t motor_id);
        ~Motor();
        void setPercent(float wantedPercent);
        float getPercent();
        void forceStop();
    private:
        Servo m_motor;
        uint8_t m_motor_id;
        float m_currPercent;
        int m_direction;

        void updateMotor();
};
#endif
