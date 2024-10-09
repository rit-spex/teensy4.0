#ifndef CONSTANTS_H
#define CONSTANTS_H

// the larger number will led to slower speeds
#define STATUS_LIGHT_FREQUENCY_MS 200

// Update rate for the rover
#define UPDATE_RATE_MS 40

//The max speed of the motors
#define PERCENT_MAX 0.45

// Max RPM of the motors
#define MAX_RPM 5000

// the PMW values of the sparkMAX
#define SPARK_MAX_MAX_SPEED (1500 + 500 * PERCENT_MAX)
#define MIN_FORWARD 1525
#define NEUTRAL 1500
#define MIN_REVERSE 1475
#define SPARK_MAX_MIN_SPEED (1500 - 500 * PERCENT_MAX)

//The max the speeds on each side can differ before the rover tips
#define MAX_DIFFERENCE_PERCENT 0.07
#define NUM_WHEELS 6
#define COUNTS_PER_REV 2048

//milliseconds in a minute
#define MS_IN_MIN 60000.0

// The Direction of the motor
#define LEFTDIRECTION  -1 //positive 1 or negitive -1
#define RIGHTDIRECTION 1 //positive 1 or negitive -1

// the time it takes to go from 0 to 100
#define RAMP_UP_TIME   3000 //milliseconds
#define RAMP_DOWN_TIME 3000  //milliseconds

// The percent Increase or Decrease per cycle
#define RAMP_UP_RATE_PERCENT   ((1.00)/(RAMP_UP_TIME/UPDATE_RATE_MS))
#define RAMP_DOWN_RATE_PERCENT ((1.00)/(RAMP_DOWN_TIME/UPDATE_RATE_MS))

// I2C device number for ARM
#define BASE_I2C_ID 15 // brushed motor
#define BASE_MAX_SPEED 800
#define CLAW_I2C_ID 16 // stepper motor
#define CLAW_MAX_SPEED (200 * 10000) // 100 steps per second

// FAN CONSTANTS
#define NUM_THERMISTORS 4
#define NUM_FANS 4

// this is in celsius
#define MAX_TEMP 44 //110 ferinheight
#define MIN_TEMP 15 // 60 ferinheight

//this is duty cycle pwm Signal
#define MAX_FAN_SPEED 255
#define MIN_FAN_SPEED 51

namespace PIDConstants {
    constexpr double KP0 = 0.001;
    constexpr double KP1 = 0.001;
    constexpr double KP2 = 0.001;
    constexpr double KP3 = 0.001;
    constexpr double KP4 = 0.001;
    constexpr double KP5 = 0.001;

    constexpr double KI0 = 0.0;
    constexpr double KI1 = 0.0;
    constexpr double KI2 = 0.0;
    constexpr double KI3 = 0.0;
    constexpr double KI4 = 0.0;
    constexpr double KI5 = 0.0;

    constexpr double KD0 = 0.00;
    constexpr double KD1 = 0.00;
    constexpr double KD2 = 0.00;
    constexpr double KD3 = 0.00;
    constexpr double KD4 = 0.00;
    constexpr double KD5 = 0.00;
};

#endif // CONSTANTS_H
