#ifndef CONSTANTS_H
#define CONSTANTS_H

//********************************************************* GENERAL CONSTANTS *******************************************************
// the larger number will led to slower frequency
#define MBB_STATUS_LIGHT_FREQUENCY_MS 200

// Update rate for the rover
#define UPDATE_RATE_MS 40

//milliseconds in a minute
#define MS_IN_MIN 60000

//********************************************************* DRIVETRAIN CONSTANTS *******************************************************
//The max percent of the motors
#define MOTOR_MAX_PERCENT 0.7

// Max RPM of the motors max 1100 with no load
#define MOTOR_MAX_RPM 1100

// the PMW values of the sparkMAX
#define SPARK_MAX_MAX_DUTY_CYCLE (1500 + 500 * MOTOR_MAX_PERCENT)
#define SPARK_MAX_MIN_FORWARD 1525 // NOT USED
#define SPARK_MAX_NEUTRAL_DUTY_CYCLE 1500
#define SPARK_MAX_MIN_REVERSE 1475 // NOT USED
#define SPARK_MAX_MIN_DUTY_CYCLE (1500 - 500 * MOTOR_MAX_PERCENT)

#define COUNTS_PER_REV 2048

// number of wheels on the rover
#define NUM_WHEELS 6

// The Direction of the motor
#define MOTOR_LEFT_SIGN  -1 //positive 1 or negitive -1
#define MOTOR_RIGHT_SIGN 1 //positive 1 or negitive -1

// the time it takes to go from 0 to 100
#define MOTOR_RAMP_UP_TIME   3000 //milliseconds
#define MOTOR_RAMP_DOWN_TIME 3000  //milliseconds

// The percent Increase or Decrease per cycle
#define MOTOR_RAMP_UP_RATE_PERCENT   ((1.00)/(MOTOR_RAMP_UP_TIME/UPDATE_RATE_MS))
#define MOTOR_RAMP_DOWN_RATE_PERCENT ((1.00)/(MOTOR_RAMP_DOWN_TIME/UPDATE_RATE_MS))

//The max the percentage on each side can differ before the rover tips
#define DRIVEBASE_MAX_DIFFERENCE_PERCENT 0.07

namespace PIDConstants {
    constexpr double KP0 = 0.01;
    constexpr double KP1 = 0.01;
    constexpr double KP2 = 0.01;
    constexpr double KP3 = 0.01;
    constexpr double KP4 = 0.01;
    constexpr double KP5 = 0.01;

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

#define MOTORS_DIR       (int[NUM_WHEELS]) {MOTOR_LEFT_SIGN, MOTOR_LEFT_SIGN, MOTOR_LEFT_SIGN, MOTOR_RIGHT_SIGN, MOTOR_RIGHT_SIGN, MOTOR_RIGHT_SIGN}
#define PID_CONSTANTS_KP (double[NUM_WHEELS]){PIDConstants::KP0, PIDConstants::KP1, PIDConstants::KP2, PIDConstants::KP3, PIDConstants::KP4, PIDConstants::KP5}
#define PID_CONSTANTS_KI (double[NUM_WHEELS]){PIDConstants::KI0, PIDConstants::KI1, PIDConstants::KI2, PIDConstants::KI3, PIDConstants::KI4, PIDConstants::KI5}
#define PID_CONSTANTS_KD (double[NUM_WHEELS]){PIDConstants::KD0, PIDConstants::KD1, PIDConstants::KD2, PIDConstants::KD3, PIDConstants::KD4, PIDConstants::KD5}

//********************************************************* ARM CONSTANTS *******************************************************
// I2C device number for ARM
#define ARM_BASE_I2C_ID 15 // brushed motor
#define ARM_BASE_MAX_SPEED 800
#define ARM_CLAW_I2C_ID 16 // stepper motor
#define ARM_CLAW_MAX_SPEED (200 * 10000) // 100 steps per second

//********************************************************* TEMP CONSTANTS *******************************************************
// FAN CONSTANTS
#define NUM_THERMISTORS 4
#define NUM_FANS 4

// this is in celsius
#define TEMP_MAX 44 //110 ferinheight
#define TEMP_MIN 15 // 60 ferinheight

//this is duty cycle pwm Signal
#define FAN_MAX_PWM 255
#define FAN_MIN_PWM 51

//********************************************************* CAN CONSTANTS *******************************************************
#define ActiveCAN CAN::CAN_MODE::CAN2
#define CAN_BAUDRATE 500000
#define CAN_MSG_LENGTH 8

//******************************************************** XBEE CONSTANTS *******************************************************
#define XBEE_NO_MESSAGE_ERROR_NUM 25 // 1 sec

#endif // CONSTANTS_H
