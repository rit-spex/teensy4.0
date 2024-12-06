#ifndef FAN_H
#define FAN_H

#include "Pinout.h"
#include "DEBUG.h"

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Arduino.h>
#endif
class Fan
{
    public:
    Fan(uint8_t fan_id);
    void setPower(int pwm_signal);
    //pwm pin to control fan speed
    // four fans
    private:
    uint8_t m_fan_id;
};

#endif