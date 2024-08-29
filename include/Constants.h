#ifndef CONSTANTS_H
#define CONSTANTS_H

// the larger number will led to slower speeds
#define STATUS_LIGHT_FREQUENCY_MS 1000

// Update rate for the rover
#define UPDATE_RATE_MS 40

//Keep this an int value
#define PERCENT_MAX 30

// Max RPM of the motors
#define MAX_RPM 5000

// the PMW values of the sparkMAX
#define SPARK_MAX_MAX_SPEED (1500 + 5 * PERCENT_MAX)
#define MIN_FORWARD 1525 //Unused
#define NEUTRAL 1500
#define MIN_REVERSE 1475 //Unused
#define SPARK_MAX_MIN_SPEED (1500 - 5 * PERCENT_MAX)

//The max the speeds on each side can differ before the rover tips
#define MAX_DIFFERENCE 0.07
#define NUM_WHEELS 6
#define COUNTS_PER_REV 2048

// I2C device number
#define BASE_I2C_ID 15 // brushed motor
#define BASE_MAX_SPEED 800
#define CLAW_I2C_ID 16 // stepper motor
#define CLAW_MAX_SPEED (200 * 10000) // 100 steps per second

namespace PIDConstants {
    constexpr double KP0 = 1.0;
    constexpr double KP1 = 1.0;
    constexpr double KP2 = 1.0;
    constexpr double KP3 = 1.0;
    constexpr double KP4 = 1.0;
    constexpr double KP5 = 1.0;

    constexpr double KI0 = 0.1;
    constexpr double KI1 = 0.1;
    constexpr double KI2 = 0.1;
    constexpr double KI3 = 0.1;
    constexpr double KI4 = 0.1;
    constexpr double KI5 = 0.1;

    constexpr double KD0 = 0.01;
    constexpr double KD1 = 0.01;
    constexpr double KD2 = 0.01;
    constexpr double KD3 = 0.01;
    constexpr double KD4 = 0.01;
    constexpr double KD5 = 0.01;
};

#endif // CONSTANTS_H