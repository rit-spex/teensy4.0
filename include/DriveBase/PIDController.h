#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "Constants.h"
#include <cstdint>

class PIDController {
public:
    // Constructor
    PIDController(uint8_t PID_id);

    // Update method
    double update(double targeted_RPM, double current_RPM);

private:
    uint8_t m_PID_id;
    double m_kp;
    double m_ki;
    double m_kd;
    double m_integral;
    double m_previous_error;
};

#endif // PID_CONTROLLER_H
