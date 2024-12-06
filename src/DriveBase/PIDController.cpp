#include "../../include/PIDController.h"
#include <Arduino.h>
#include "../../include/Constants.h"

// Constructor
PIDController::PIDController(double kp, double ki, double kd)
    : kp(kp), ki(ki), kd(kd), integral(0), previous_error(0), previous_output(0) {}

// Update method
double PIDController::update(double targeted_RPM, double current_RPM) {
    double error = current_RPM - targeted_RPM;
    integral += error*UPDATE_RATE_SEC;
    double derivative = error - previous_error;
    double output = kp * error + ki * integral + kd * (derivative/UPDATE_RATE_SEC) + previous_output;
    previous_output = output;
    previous_error = error;
    Serial.print("error: ");
    Serial.print(error);

    Serial.print(" prev error: ");
    Serial.print(previous_error);

    Serial.print(" prev out: ");
    Serial.print(previous_output);

    Serial.print(" out: ");
    Serial.println(output);

    return output;
}
