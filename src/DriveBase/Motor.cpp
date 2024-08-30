#include "../../include/Motor.h"

//constructor
Motor::Motor(PWM_PINS pwm_pin, int direction)
{
    m_motor.attach(pwm_pin, SPARK_MAX_MIN_SPEED, SPARK_MAX_MAX_SPEED);  // Assuming 'motor' is a member variable of the Motor class
    m_speed = 0;
    m_direction = direction;
}

Motor::~Motor()
{
    //clear the motor pin
    m_motor.detach();
}

//write to the motor
void Motor::setSpeed(float percent)
{
    Serial.println(percent);
    //First check if the target speed is within the bounds of -1 to 1
    if(percent < -PERCENT_MAX)
    {
        percent = -PERCENT_MAX;
    }
    else if(percent > PERCENT_MAX)
    {
        percent = PERCENT_MAX;
    }
    else
    {
        percent = percent;
    }

    // if the projected speed is greater than current speed ramp use ramp up
    if(RAMP_UP_RATE_PERCENT < fabs(percent) - fabs(m_speed))
    {
        if(percent < 0)
        {
            m_speed = m_speed - RAMP_UP_RATE_PERCENT;
        }
        else
        {
            m_speed = m_speed + RAMP_UP_RATE_PERCENT;
        }
    }
    // if the projected speed is less than current speed use ramp down
    else if(RAMP_DOWN_RATE_PERCENT < fabs(m_speed) - fabs(percent) )
    {
        if(percent < 0)
        {
            m_speed = m_speed + RAMP_DOWN_RATE_PERCENT;
        }
        else
        {
            m_speed = m_speed - RAMP_DOWN_RATE_PERCENT;
        }
    }
    else
    {
        m_speed = percent;
    }



    // If the target speed is negative, set the PWM duty cycle to the reverse range from 1500(0%) to 1000(-100%)
    if(percent < 0)
    {
        m_motor.writeMicroseconds(NEUTRAL - floor((SPARK_MAX_MIN_SPEED - NEUTRAL) * m_speed * m_direction));
    }
    // If the target speed is positive, set the PWM duty cycle to the forward range from 1500(0%) to 2000(100%)
    else if(percent > 0)
    {
        m_motor.writeMicroseconds(NEUTRAL + floor((SPARK_MAX_MAX_SPEED - NEUTRAL) * m_speed * m_direction));
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
