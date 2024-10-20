#ifndef PINOUT_H
#define PINOUT_H

//********************************************************* GENERAL PINOUT *******************************************************
#define STATUS_LIGHT_PIN 13


//********************************************************* DRIVETRAIN PINOUT ****************************************************

enum PWM_PINS {
    PWM_PIN_0 = 2,
    PWM_PIN_1 = 3,
    PWM_PIN_2 = 4,
    PWM_PIN_3 = 12, // change to 12
    PWM_PIN_4 = 10,
    PWM_PIN_5 = 11
};

enum ENC_A_PINS {
    ENC_A_PIN_0 = 30,
    ENC_A_PIN_1 = 9,//not valid
    ENC_A_PIN_2 = 10,//not valid
    ENC_A_PIN_3 = 11,//not valid
    ENC_A_PIN_4 = 12,//not valid
    ENC_A_PIN_5 = 13 //not valid
};

enum ENC_B_PINS {
    ENC_B_PIN_0 = 31,
    ENC_B_PIN_1 = 15,//not valid
    ENC_B_PIN_2 = 16,//not valid
    ENC_B_PIN_3 = 17,//not valid
    ENC_B_PIN_4 = 18,//not valid
    ENC_B_PIN_5 = 19//not valid
};

//********************************************************* CAN PINOUT *******************************************************

enum CAN_PINS {
    CAN_RX_PIN = 0,
    CAN_TX_PIN = 1
};

//********************************************************* TEMP PINOUT *******************************************************

enum THERMISTOR_PINS {
    THERMISTOR_PIN_0 = 20,
    THERMISTOR_PIN_1 = 21,
    THERMISTOR_PIN_2 = 22,
    THERMISTOR_PIN_3 = 23
};

enum FAN_PINS {
    FAN_PIN_0 = 22,
    FAN_PIN_1 = 23,
    FAN_PIN_2 = 24,
    FAN_PIN_3 = 25
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
