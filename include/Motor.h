#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Servo.h>
#include "Pinout.h"
#include "Constants.h"

// Motor class for controlling the motors, this is meant to be a generic class for all motors
class Motor {
    public:
        Motor(PWM_PINS pwm_pin);
        ~Motor();
        void setSpeed(float percent);
        float getSpeed();
    private:
        Servo m_motor;
        float m_speed;
};
#endif
