#include "../../include/DriveBase/PIDController.h"

// Constructor
PIDController::PIDController(uint8_t PID_id)
    : m_PID_id(PID_id), m_kp(PID_CONSTANTS_KP[PID_id]), m_ki(PID_CONSTANTS_KI[PID_id]), m_kd(PID_CONSTANTS_KD[PID_id]), m_integral(0), m_previous_error(0) {}

// Update method
double PIDController::update(double targeted_RPM, double current_RPM) {
    double error = targeted_RPM - current_RPM;
    m_integral += error;
    double derivative = error - m_previous_error;
    double output = m_kp * error + m_ki * m_integral + m_kd * derivative;
    m_previous_error = error;
    return output;
}
