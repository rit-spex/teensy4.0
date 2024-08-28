#include "../../include/Motor.h"

Motor::Motor(PWM_PINS pwm_pin)
{
    this->pwm_pin = pwm_pin;
    //pinMode((int)this->pwm_pin, OUTPUT);
    motor.attach(this->pwm_pin, SPARK_MAX_MIN, SPARK_MAX_MAX);  // Assuming 'motor' is a member variable of the Motor class
}

Motor::~Motor()
{
    motor.detach();
}

void Motor::setSpeed(float duty_cycle_us) 
{
    motor.writeMicroseconds(duty_cycle_us);
}
