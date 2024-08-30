#include "../include/MainBodyBoard.h"

MainBodyBoard::MainBodyBoard()
{
    pinMode(STATUS_LIGHT_PIN, OUTPUT);
}

MainBodyBoard::~MainBodyBoard()
{

}

void MainBodyBoard::startUp()
{
    // set the status light to off
    digitalWrite(STATUS_LIGHT_PIN, HIGH);
    m_statusLightWait = floor(STATUS_LIGHT_FREQUENCY_MS/UPDATE_RATE_MS);
    m_statusLightOn = true;
}

void MainBodyBoard::BlinkStatusLight()
{
    // blink the status light every STATUS_LIGHT_FREQUENCY_MS
    if(m_statusLightWait <= 0)
    {
        if(m_statusLightOn)
        {
            digitalWrite(STATUS_LIGHT_PIN, LOW);
            m_statusLightOn = false;
        }
        else
        {
            digitalWrite(STATUS_LIGHT_PIN, HIGH);
            m_statusLightOn = true;
        }
        m_statusLightWait = floor(STATUS_LIGHT_FREQUENCY_MS/UPDATE_RATE_MS);
    }
    else
    {
        m_statusLightWait--;
    }
}

void MainBodyBoard::updateSubsystems(int timeInterval_ms)
{
    BlinkStatusLight();

    // uint8_t locationA[8] = {35,122,96,00,64,66,15,00};
    // uint8_t locationB[8] = {35,122,96,00,00,00,00,00};
    // uint8_t data[8] = {0,0,0,0,0,0,0,0};
    // can.sendMessage(CAN::CAN_MB::JETSON,CAN::Message_ID::E_STOP, data);
    // can.TEST();

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

    #if ENABLE_DRIVEBASE
        #if ENABLE_ENCODER
        m_drive_base.updateRPM(timeInterval_ms);
        #endif
    #endif

    #if ENABLE_TEMP
    m_temp_subsystem.updateFans();
    #endif
}

#if ENABLE_DRIVEBASE
#if MASTER_TEENSY
void MainBodyBoard::drive(float left_axis, float right_axis)
{

    m_drive_base.drive(left_axis, right_axis);

}
#endif
#endif
