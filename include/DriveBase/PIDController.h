#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#define OUTPUT_MIN 0
#define OUTPUT_MAX 500

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
    double kp;
    double ki;
    double kd;
    double integral;
    double previous_error;
};

#endif // PID_CONTROLLER_H
