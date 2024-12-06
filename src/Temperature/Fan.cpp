#include "../../include/Temperature/Fan.h"

Fan::Fan(uint8_t fan_id) : m_fan_id(fan_id)
{
    pinMode(FAN_PINS[fan_id], OUTPUT);
}

void Fan::setPower(int pwm_signal)
{
    analogWrite(FAN_PINS[m_fan_id], pwm_signal);
    // four fans
}