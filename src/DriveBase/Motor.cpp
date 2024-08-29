#include "../../include/Motor.h"

//constructor
Motor::Motor(PWM_PINS pwm_pin)
{
    m_motor.attach(pwm_pin, SPARK_MAX_MIN_SPEED, SPARK_MAX_MAX_SPEED);  // Assuming 'motor' is a member variable of the Motor class
    m_speed = 0;
}

Motor::~Motor()
{
    //clear the motor pin
    m_motor.detach();
}

//write to the motor
void Motor::setSpeed(float percent) 
{
    //First check if the target speed is within the bounds of -1 to 1
    if(percent < -1.0)
    {
        m_speed = -1.0;
    }
    else if(percent > 1.0)
    {
        m_speed = 1.0;
    }
    else
    {
        m_speed = percent;
    }

    // If the target speed is negative, set the PWM duty cycle to the reverse range from 1500(0%) to 1000(-100%)
    if(percent < 0)
    {
        m_motor.writeMicroseconds(NEUTRAL - (SPARK_MAX_MIN_SPEED - NEUTRAL) * m_speed);
    }
    // If the target speed is positive, set the PWM duty cycle to the forward range from 1500(0%) to 2000(100%)
    else if(percent > 0)
    {
        m_motor.writeMicroseconds(NEUTRAL + (SPARK_MAX_MAX_SPEED - NEUTRAL) * m_speed);
    }
    // If the target speed is 0, set the PWM duty cycle to neutral
    else
    {
        m_motor.writeMicroseconds(NEUTRAL);
    }
}

// return the speed that the motor is set to
float Motor::getSpeed()
{
    return m_speed;
}
