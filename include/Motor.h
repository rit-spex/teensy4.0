#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Servo.h>
#include "Motor.h"
#include "Pinout.h"

#define PERCENT_MAX 0.3

#define SPARK_MAX_MIN (1500 - 500 * PERCENT_MAX)
#define SPARK_MAX_MAX (1500 + 500 * PERCENT_MAX)

class Motor {
    public:
        Motor(PWM_PINS pwm_pin);
        ~Motor();
        void setSpeed(float duty_cycle_us);
    private:
        int pwm_pin;
        Servo motor;
};
#endif
