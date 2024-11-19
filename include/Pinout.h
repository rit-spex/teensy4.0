#ifndef PINOUT_H
#define PINOUT_H

//********************************************************* GENERAL PINOUT *******************************************************
#define STATUS_LIGHT_PIN 13


//********************************************************* DRIVETRAIN PINOUT ****************************************************

enum PWM_PINS {
    PWM_PIN_0 = 2,
    PWM_PIN_1 = 3,
    PWM_PIN_2 = 4,
    PWM_PIN_3 = 12,//5, // is 12 on proto-board
    PWM_PIN_4 = 10,//6, // is 10 on proto-board
    PWM_PIN_5 = 11//7  // is 11 on proto-board
};

enum ENC_A_PINS {
    ENC_A_PIN_0 = 31,//8, // is 31 on proto-board
    ENC_A_PIN_1 = 10,
    ENC_A_PIN_2 = 12,
    ENC_A_PIN_3 = 22,
    ENC_A_PIN_4 = 20,
    ENC_A_PIN_5 = 16 
};

enum ENC_B_PINS {
    ENC_B_PIN_0 = 30,//9, // is 30 on proto-board
    ENC_B_PIN_1 = 11,
    ENC_B_PIN_2 = 23,
    ENC_B_PIN_3 = 21,
    ENC_B_PIN_4 = 17,
    ENC_B_PIN_5 = 15
};

//********************************************************* CAN PINOUT *******************************************************

enum CAN_PINS {
    CAN_RX_PIN = 0,
    CAN_TX_PIN = 1
};

//********************************************************* TEMP PINOUT *******************************************************

enum THERMISTOR_PINS {
    THERMISTOR_PIN_0 = 24,
    THERMISTOR_PIN_1 = 25,
    THERMISTOR_PIN_2 = 26,
    THERMISTOR_PIN_3 = 27
};

enum FAN_PINS {
    FAN_PIN_0 = 28, // not valid
    FAN_PIN_1 = 23, // not valid
    FAN_PIN_2 = 24, // not valid
    FAN_PIN_3 = 25  // not valid
};

//********************************************************* ARM PINOUT  *******************************************************
// define the I2C pins for the arm
#define ARM_SDA_PIN 18
#define ARM_SCL_PIN 19

// define these pins, wristDir and shoulderDir can be any digital output
// wristSpeed and shoulderSpeed need to be pins 9 and 6
#define WRIST_DIR_PIN 33
#define WRIST_SPEED_PIN 6 // MUST BE PIN 6
#define SHOULDER_DIR_PIN 34
#define SHOULDER_SPEED_PIN 9 // MUST BE PIN 9

#endif
