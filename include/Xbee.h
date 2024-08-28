#ifndef XBEE_H
#define XBEE_H

#include <map>
#include "Pinout.h"

#define SAVE_SIZE 3
#define START_COMMAND 0xde

//static int bigserialbuffer[16384];

class Xbee
{
private:
    int numNoSignal;
    int lastSignalCount;
    bool isDisabled = false;
    bool firstConnected = false;

    float findAxisMedian(int index);
    bool findButtonMedian(int index);

    void intToBinary(int n, bool *bin);

    bool buttonvalues[8][SAVE_SIZE];
    float axisvalues[2][SAVE_SIZE];

    //std::map<int, bool> translate =
    //{
    //    {0xcf , 1}, {0xcd , 0}, //A
    //    {0xd1 , 1}, {0xd2 , 0}, //B
    //    {0xd3 , 1}, {0xd4 , 0}, //X
    //    {0xd5 , 1}, {0xc6 , 0}, //Y
    //    {0xd7 , 1}, {0xc8 , 0}, //LB
    //    {0xd9 , 1}, {0xca , 0}, //RB
    //    {0xdb , 1}, {0xcc , 0}, //SELECT
    //    {0xdd , 1}, {0xce , 0}, //START
    //    {0xe4 , 1}, {0xe3 , 0}  //HOME
    //};

    // change what data to update next
    int currentHead = 0;

public:
    long long error_count = 0;
    long long good_count = 0;

    enum CONTROLLER
    {
        LEFT_Y_AXIS = 0, // left drive 
        RIGHT_Y_AXIS = 1, //right drive
        LEFT_TRIGGER = 2, // j1 left
        RIGHT_TRIGGER = 3, // j1 right
        A_BUTTON = 4, // grip open
        B_BUTTON = 5, // grip close
        X_BUTTON = 6, // j3 down
        Y_BUTTON = 7, // j3 up
        LB_BUTTON = 8, // j2 down
        RB_BUTTON = 9, // j2 up
    };

    Xbee();
    void UpdateValues();
    float getCurrentValue(CONTROLLER controller);
};


#endif