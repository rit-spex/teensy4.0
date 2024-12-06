#include "../../include/DriveBase/PIDController.h"

// Constructor
PIDController::PIDController(uint8_t PID_id)
    : m_PID_id(PID_id), kp(PID_CONSTANTS_KP[PID_id]), ki(PID_CONSTANTS_KI[PID_id]), kd(PID_CONSTANTS_KD[PID_id]), integral(0), previous_error(0) {}

// Update method
double PIDController::update(double targeted_RPM, double current_RPM) {
    double error = targeted_RPM - current_RPM;
    integral += error;
    double derivative = error - previous_error;
    double output = kp * error + ki * integral + kd * derivative;
    previous_error = error;
    return output;
}
