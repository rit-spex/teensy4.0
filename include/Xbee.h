#ifndef XBEE_H
#define XBEE_H

#include "DEBUG.h"
#include "Pinout.h"
#include <map>

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#endif

#define SAVE_SIZE 3
#define START_COMMAND 0xde

#define NUM_AXES 2
#define NUM_BUTTONS 8


//static int bigserialbuffer[16384];

class Xbee
{
private:

    // the amount of cycles with no signal
    int numNoSignal;

    // a flag if the xbee is disabled
    bool isDisabled = false;
    
    int lastSignalCount;

    // a flag that once triggered will start the xbee
    bool firstConnected = false;

    // parse messages from the xbee
    void parseMessage();

    // find the median of the values in the buffer, will prevent errors
    float findAxisMedian(int index);
    bool findButtonMedian(int index);

    // convert an int to a binary array
    void intToBinary(int n, bool *bin);

    // change what data in the buffer update next
    int currentHead = 0;

    // all of the current values
    bool buttonvalues[NUM_BUTTONS][SAVE_SIZE];
    float axisvalues[NUM_AXES][SAVE_SIZE];

    // flag if new values are found
    bool isNewValuesFound = false;

    // the current values
    float currentValues[NUM_BUTTONS + NUM_AXES];

public:
    long long error_count = 0;
    long long good_count = 0;

    enum CONTROLLER
    {
        LEFT_Y_AXIS   = 0, // left drive 
        RIGHT_Y_AXIS  = 1, //right drive
        LEFT_TRIGGER  = 2, // j1 left
        RIGHT_TRIGGER = 3, // j1 right
        A_BUTTON      = 4, // grip open
        B_BUTTON      = 5, // grip close
        X_BUTTON      = 6, // j3 down
        Y_BUTTON      = 7, // j3 up
        LB_BUTTON     = 8, // j2 down
        RB_BUTTON     = 9, // j2 up
    };

    Xbee();
    void UpdateValues();

    float getCurrentValue(CONTROLLER controller);
    bool isActive();
    bool isNewValues();
    void printValues();
};


#endif