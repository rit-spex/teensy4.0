#include "../include/MainBodyBoard.h"

MainBodyBoard::MainBodyBoard(){}

MainBodyBoard::~MainBodyBoard(){}

void MainBodyBoard::startUp()
{
    // set up the status light
    pinMode(STATUS_LIGHT_PIN, OUTPUT);

    // set the status light to stay on
    digitalWrite(STATUS_LIGHT_PIN, HIGH);
    m_statusLightWait = floor(STATUS_LIGHT_FREQUENCY_MS/UPDATE_RATE_MS);
    m_statusLightOn = true;
    
    #if ENABLE_CAN
    m_can.startCAN();
    #endif
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
    #if ENABLE_CAN
    m_disabled = m_can.getUnpackedMessage(CAN::Message_ID::E_STOP);
    #endif

    if(!m_disabled)
    {
        BlinkStatusLight();
        #if ENABLE_DRIVEBASE
            #if ENABLE_ENCODER
            m_drive_base.updateRPM(timeInterval_ms);
            #endif
        #endif

        #if ENABLE_TEMP
        m_temp_subsystem.updateFans();
        #endif
    }
    else
    {
        #if ENABLE_SERIAL
        Serial.println("DISABLED");
        #endif
    }
}

void MainBodyBoard::runBackgroundProcess(void)
{
    #if ENABLE_CAN
    m_can.readMsgBuffer();
    #endif
}


#if ENABLE_DRIVEBASE
#if MASTER_TEENSY
void MainBodyBoard::drive(float left_axis, float right_axis)
{
    if(!m_disabled)
    {
        m_drive_base.drive(left_axis, right_axis);
    }
}
#endif
#endif

void MainBodyBoard::disable()
{
    // disable the rover and stop everything
    m_disabled = true;

    // set the led to solid
    digitalWrite(STATUS_LIGHT_PIN, HIGH);

    #if ENABLE_DRIVEBASE
    m_drive_base.forceStop();
    #endif
}

bool MainBodyBoard::isDisabled()
{
    return m_disabled; 
}
