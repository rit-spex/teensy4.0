#ifndef XBEE_H
#define XBEE_H

#include "DEBUG.h"
#include "Pinout.h"
#include "Constants.h"
#include <map>

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#endif

#define SAVE_SIZE 1
#define START_COMMAND 0xde

#define NUM_AXES 2
#define NUM_BUTTONS 8


//static int bigserialbuffer[16384];

class Xbee
{
private:

    // the amount of cycles with no signal
    int m_numNoSignal;

    // a flag if the xbee is disabled
    bool m_isDisabled = false;
    
    // a flag that once triggered will start the xbee
    bool m_firstConnected = false;

    // change what data in the buffer update next
    int m_currentHead = 0;

    // all of the current values
    bool m_buttonvalues[NUM_BUTTONS][SAVE_SIZE];
    float m_axisvalues[NUM_AXES][SAVE_SIZE];

    // the current values
    float m_currentValues[NUM_BUTTONS + NUM_AXES];

    // flag if new values are found
    bool m_isNewValuesFound = false;

    // parse messages from the xbee
    void parseMessage();

    // find the median of the values in the buffer, will prevent errors
    float findAxisMedian(int index);
    bool findButtonMedian(int index);

    // convert an int to a binary array
    void intToBinary(int n, bool *bin);

public:
    enum CONTROLLER
    {
        LEFT_Y_AXIS   = 0, // left drive 
        RIGHT_Y_AXIS  = 1, //right drive
        A_BUTTON      = 2, // grip open
        B_BUTTON      = 3, // grip close
        X_BUTTON      = 4, // j3 down
        Y_BUTTON      = 5, // j3 up
        LB_BUTTON     = 6, // j2 down
        RB_BUTTON     = 7, // j2 up
        LEFT_TRIGGER  = 8, // j1 left
        RIGHT_TRIGGER = 9, // j1 right
    };

    Xbee();
    void UpdateValues();

    float getCurrentValue(CONTROLLER controller);
    bool isDisabled();
    bool isNewValues();
    void printValues();
};


#endif