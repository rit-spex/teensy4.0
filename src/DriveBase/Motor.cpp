#include "../../include/DriveBase/Motor.h"

//constructor
Motor::Motor(uint8_t motor_id)
{
    m_motor_id = motor_id;
    m_motor.attach(MOTOR_PWM_PINS[motor_id], SPARK_MAX_MIN_DUTY_CYCLE, SPARK_MAX_MAX_DUTY_CYCLE);  // Assuming 'motor' is a member variable of the Motor class
    m_currPercent = 0;
    m_direction = MOTORS_DIR[m_motor_id];
}

Motor::~Motor()
{
    //clear the motor pin
    m_motor.detach();
}

//write to the motor
void Motor::setPercent(float wantedPercent)
{
    //First check if the target speed is within the bounds of -1 to 1
    if(wantedPercent < -MOTOR_MAX_PERCENT)
    {
        wantedPercent = -MOTOR_MAX_PERCENT;
    }
    else if(wantedPercent > MOTOR_MAX_PERCENT)
    {
        wantedPercent = MOTOR_MAX_PERCENT;
    }

    #if ENABLE_MOTOR_RAMP
    // apply ramp rules
    if(m_currPercent > 0)
    {
        if(wantedPercent > (m_currPercent + MOTOR_RAMP_UP_RATE_PERCENT))
        {
            m_currPercent += MOTOR_RAMP_UP_RATE_PERCENT;
        }
        else if(wantedPercent < (m_currPercent - MOTOR_RAMP_DOWN_RATE_PERCENT))
        {
            m_currPercent -= MOTOR_RAMP_DOWN_RATE_PERCENT;
        }
        else
        {
            m_currPercent = wantedPercent;
        }
    }
    else if(m_currPercent < 0)
    {
        if(wantedPercent < (m_currPercent - MOTOR_RAMP_UP_RATE_PERCENT))
        {
            m_currPercent -= MOTOR_RAMP_UP_RATE_PERCENT;
        }
        else if(wantedPercent > (m_currPercent + MOTOR_RAMP_DOWN_RATE_PERCENT))
        {
            m_currPercent += MOTOR_RAMP_DOWN_RATE_PERCENT;
        }
        else
        {
            m_currPercent = wantedPercent;
        }
    }
    else
    {
        if(MOTOR_RAMP_UP_RATE_PERCENT < wantedPercent)
        {
            m_currPercent += MOTOR_RAMP_UP_RATE_PERCENT;
        }
        else if(MOTOR_RAMP_UP_RATE_PERCENT < -wantedPercent)
        {
            m_currPercent -= MOTOR_RAMP_UP_RATE_PERCENT;
        }
        else
        {
            m_currPercent = wantedPercent;
        }
    }
    #else // Disable ramp rate
    
    // set the current percent to the wanted percent
    m_currPercent = wantedPercent;
    #endif

   updateMotor();
}

// return the speed that the motor is set to
float Motor::getPercent()
{
    return m_currPercent;
}

void Motor::forceStop()
{
    m_currPercent = 0;
    updateMotor();
}

void Motor::updateMotor()
{
    // If the target percent is negative, set the PWM duty cycle to the reverse range from 1500(0%) to 1000(-100%)
    if(m_currPercent < 0)
    {
        m_motor.writeMicroseconds(NEUTRAL - floor((SPARK_MAX_MIN_DUTY_CYCLE - NEUTRAL) * m_currPercent * m_direction));
    }
    // If the target percent is positive, set the PWM duty cycle to the forward range from 1500(0%) to 2000(100%)
    else if(m_currPercent > 0)
    {
        m_motor.writeMicroseconds(NEUTRAL + floor((SPARK_MAX_MAX_DUTY_CYCLE - NEUTRAL) * m_currPercent * m_direction));
    }
    // If the target percent is 0, set the PWM duty cycle to neutral
    else
    {
        m_motor.writeMicroseconds(NEUTRAL);
    }
}