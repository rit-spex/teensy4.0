#include "../../include/DriveBase/PIDController.h"

// Constructor
PIDController::PIDController(double kp, double ki, double kd)
    : kp(kp), ki(ki), kd(kd), integral(0), previous_error(0) {}

// Update method
double PIDController::update(double targeted_RPM, double current_RPM) {
    double error = targeted_RPM - current_RPM;
    integral += error;
    double derivative = error - previous_error;
    double output = kp * error + ki * integral + kd * derivative;
    previous_error = error;
    return output;
}
