#include "../../include/DriveBase/Wheel.h"

#if ENABLE_ENCODER
Wheel::Wheel(uint8_t wheel_id)
     : m_motor(wheel_id), m_encoder(wheel_id), m_pid(wheel_id){

     this->m_targetRPM = 0;
     this->m_currentRPM = 0;
}
#else
Wheel::Wheel(uint8_t wheel_id):
    m_motor(wheel_id){}
#endif

void Wheel::setPercent(float targetPercent)
{
    m_motor.setPercent(targetPercent);
}

#if ENABLE_ENCODER
void Wheel::setRPM(float targetRPM)
{
    this->m_targetRPM = targetRPM;
}

float Wheel::getRPM()
{
    return this->m_currentRPM;
}

/*
* Updates the PID controller for the wheel
*/
void Wheel::updatePID(int timeInterval_ms)
{
    this->m_currentRPM = this->m_encoder.getRPM(timeInterval_ms);
    float pidOutput = this->m_pid.update(this->m_targetRPM, this->m_currentRPM);
    this->setPercent(pidOutput);
}

#endif

void Wheel::forceStop()
{
    m_motor.forceStop();
}