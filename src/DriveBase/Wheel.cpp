#include "../../include/Wheel.h"

#if ENABLE_ENCODER
Wheel::Wheel(int wheel_num, PWM_PINS pwm_pin, int direction, double kp, double ki, double kd)
     : m_motor(pwm_pin, direction), m_encoder(wheel_num), m_pid(kp, ki, kd), wheel_num(wheel_num){

     this->m_targetRPM = 0;
     this->m_currentRPM = 0;
}
#else
Wheel::Wheel(PWM_PINS pwm_pin, int direction):
    m_motor(pwm_pin, direction){}
#endif

void Wheel::setSpeed(float targetSpeed)
{
    m_motor.setSpeed(targetSpeed);
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
    Serial.println(pidOutput);
    this->setSpeed(pidOutput);
}

#endif

void Wheel::forceStop()
{
    m_motor.forceStop();
}