#include "../include/MainBodyBoard.h"

MainBodyBoard::MainBodyBoard()
{
    #ifndef DISABLE_STATUS_LIGHT
    pinMode(STATUS_LIGHT_PIN, OUTPUT);
    #endif
}

MainBodyBoard::~MainBodyBoard()
{
    
}

void MainBodyBoard::updateSubsystems(void)
{

    digitalWrite(STATUS_LIGHT_PIN, HIGH);

    #ifndef DISABLE_STATUS_LIGHT
    if(statusLightWait == 0)
    {
        if(statusLightOn)
        {
            digitalWrite(STATUS_LIGHT_PIN, LOW);
            statusLightOn = false;
        }
        else
        {
            digitalWrite(STATUS_LIGHT_PIN, HIGH);
            statusLightOn = true;
        }
        statusLightWait = 10;
    }
    else
    {
        statusLightWait--;
    }
    #endif
    
    // uint8_t locationA[8] = {35,122,96,00,64,66,15,00};
    // uint8_t locationB[8] = {35,122,96,00,00,00,00,00};



    // uint8_t data[8] = {0,0,0,0,0,0,0,0};
    // can.sendMessage(CAN::CAN_MB::JETSON,CAN::Message_ID::E_STOP, data);
    // can.TEST();
    
    // digitalWrite(STATUS_LIGHT_PIN, HIGH);

    

    // #ifndef DISABLE_STATUS_LIGHT
    // if(can.IsEStop(can.getMessage(CAN::Message_ID::E_STOP)))
    // {
    //     statusLightWait = -1;
    //     digitalWrite(STATUS_LIGHT_PIN, HIGH);
    // }
    // else if(statusLightWait < 0)
    // {
    //     statusLightWait = 0;
    // }
    // #endif

    //#ifndef DISABLE_DRIVEBASE
    //drive_base.updateVelocity();
    //#endif

    #ifndef DISABLE_TEMP
    temp_subsystem.updateFans();
    #endif
}

void MainBodyBoard::drive(float left_axis, float right_axis)
{
    #ifndef DISABLE_DRIVEBASE
    drive_base.drive(left_axis, right_axis);
    #endif
}