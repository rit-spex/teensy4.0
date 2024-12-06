#include "../../include/Temperature/TemperatureSubsystem.h"


/*
* Constructor for the Temperature subsystem class.
* Initializes the thermistors.
*/
#if ENABLE_CAN
TemperatureSubsystem::TemperatureSubsystem(CAN *can) :
#else
TemperatureSubsystem::TemperatureSubsystem() :
#endif
    m_thermistors
    {
        Thermistor(THERMISTOR_PINS::THERMISTOR_PIN_0), 
        Thermistor(THERMISTOR_PINS::THERMISTOR_PIN_1),
        Thermistor(THERMISTOR_PINS::THERMISTOR_PIN_2),
        Thermistor(THERMISTOR_PINS::THERMISTOR_PIN_3)
    },
    m_fans
    {
        Fan(FAN_PINS::FAN_PIN_0),
        Fan(FAN_PINS::FAN_PIN_1),
        Fan(FAN_PINS::FAN_PIN_2),
        Fan(FAN_PINS::FAN_PIN_3)
    }
    #if ENABLE_CAN
    ,m_can(can)
    #endif
{}


// @return An array of temperatures measured by each thermistor
float* TemperatureSubsystem::getTemperature() 
{
    for(int i=0; i<NUM_THERMISTORS; i++)
    {
        m_temperature[i] = m_thermistors[i].getTemperature();
    }
    return m_temperature;
}

// Set the power of the fans
void TemperatureSubsystem::setFansPower(int power)
{
    power = min(max(power, MIN_FAN_SPEED), MAX_FAN_SPEED); // clamp the power to the range (MIN_FAN_SPEED, MAX_FAN_SPEED)

    for (int i = 0; i < NUM_FANS; i++)
    {
        m_fans[i].setPower(power);
    }
}

void TemperatureSubsystem::updateFans()
{
    getTemperature();
    float avgTemperature = 0;
    for (int i = 0; i < NUM_THERMISTORS; i++)
    {
        avgTemperature += m_temperature[i];
    }
    avgTemperature /= NUM_THERMISTORS;

    if(avgTemperature > MAX_TEMP)
    {
        setFansPower(MAX_FAN_SPEED);
    }
    else if(avgTemperature < MIN_TEMP)
    {
        setFansPower(MIN_FAN_SPEED);
    }
    else
    {
        float deltaTemperature = MAX_TEMP - MIN_TEMP;
        float deltaPower = MAX_FAN_SPEED - MIN_FAN_SPEED;
        
        int power = (avgTemperature - MIN_TEMP) / deltaTemperature * deltaPower + MIN_FAN_SPEED;
        setFansPower(power); // change later
    }
}