#include "../../include/DriveBase/DriveBase.h"

/*
* Constructor for the drive base class.
* Initializes the wheels of the rover.
*/

// wheels can head in different directions FIX

#if ENABLE_CAN
DriveBase::DriveBase(CAN *can):
#else
DriveBase::DriveBase():
#endif

m_wheels{ Wheel(0), Wheel(1), Wheel(2), Wheel(3), Wheel(4), Wheel(5)}

{
    #if ENABLE_CAN
    m_CAN = can;
    #endif
    #if ENABLE_ENCODER
    for(int i = 0; i<NUM_WHEELS; i++)
    {
        this->m_targetRPM[i] = 0;
    }
    #endif
}

#if ENABLE_ENCODER
// IF using teensy as main system then use this function
// Drives the rover based on the left and right joystick values
// The actual values will be calculated by the PID controller
void DriveBase::drive(float left_axis, float right_axis)
{
    // If the difference between the left and right axis is less than the max difference use normal values
    // this is to prevent the rover from tipping over
    if (fabs(fabs(left_axis) - fabs(right_axis)) < (MAX_DIFFERENCE_PERCENT/PERCENT_MAX))
    {
        m_targetRPM[0] = left_axis * MAX_RPM;
        m_targetRPM[1] = left_axis * MAX_RPM;
        m_targetRPM[2] = left_axis * MAX_RPM;
        m_targetRPM[3] = right_axis * MAX_RPM;
        m_targetRPM[4] = right_axis * MAX_RPM;
        m_targetRPM[5] = right_axis * MAX_RPM;
    }

    // If joystick value are difference is greater than the max difference
    // Adjust the values to follow the greater value
    else if (fabs(left_axis) > fabs(right_axis))
    {
        int isNegative = left_axis/fabs(left_axis);
        m_targetRPM[0] = left_axis * MAX_RPM;
        m_targetRPM[1] = left_axis * MAX_RPM;
        m_targetRPM[2] = left_axis * MAX_RPM;
        m_targetRPM[3] = (left_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative) * MAX_RPM;
        m_targetRPM[4] = (left_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative) * MAX_RPM;
        m_targetRPM[5] = (left_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative) * MAX_RPM;
    }
    else if (fabs(left_axis)<fabs(right_axis))
    {
        int isNegative = right_axis/fabs(right_axis);
        m_targetRPM[0] = (right_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative) * MAX_RPM;
        m_targetRPM[1] = (right_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative) * MAX_RPM;
        m_targetRPM[2] = (right_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative) * MAX_RPM;
        m_targetRPM[3] = right_axis * MAX_RPM;
        m_targetRPM[4] = right_axis * MAX_RPM;
        m_targetRPM[5] = right_axis * MAX_RPM;
    }
}

// Updates the velocity of the wheels to match the target velocity. This will also update PID
void DriveBase::updateRPM(int timeInterval_ms)
{
    #if ENABLE_CAN
    getTargetRPM();
    #endif
    for (int i = 0; i < NUM_WHEELS; i++)
    {
        m_wheels[i].setRPM(m_targetRPM[i]);
    }

    for (int i = 0; i < NUM_WHEELS; i++)
    {
        m_wheels[i].updatePID(timeInterval_ms);
    }

    #if ENABLE_SERIAL
    for(int i =0; i<NUM_WHEELS; i++)
    {
        Serial.print("Wheel num: ");
        Serial.print(i);
        Serial.print(" target rpm: ");
        Serial.print(m_targetRPM[i]);
        Serial.print(" current rpm: ");
        Serial.println(m_wheels[i].getRPM());

    }
    #endif

}

#if ENABLE_CAN
// Retrieves the target RPM from the CAN bus
void DriveBase::getTargetRPM()
{
    if(m_CAN->isNewMessage(CAN::Message_ID::TARGET_RPM))
    {
        for (int i = 0; i < NUM_WHEELS; i++)
        {
            m_targetRPM[i] = m_CAN->getUnpackedMessage(CAN::Message_ID::TARGET_RPM, i);    
        }
    }
}
#endif // DISABLE_CAN

#else // ENABLE_ENCODER
void DriveBase::drive(float left_axis, float right_axis)
{
    // If the difference between the left and right axis is less than the max difference use normal values
    // this is to prevent the rover from tipping over
    if (fabs(fabs(left_axis) - fabs(right_axis)) < (float)(MAX_DIFFERENCE_PERCENT/PERCENT_MAX))
    {
        updateSingleWheelSpeed(0, left_axis);
        updateSingleWheelSpeed(1, left_axis);
        updateSingleWheelSpeed(2, left_axis);
        updateSingleWheelSpeed(3, right_axis);
        updateSingleWheelSpeed(4, right_axis);
        updateSingleWheelSpeed(5, right_axis);
    }

    // If joystick value are difference is greater than the max difference
    // Adjust the values to follow the greater value
    else if (fabs(left_axis) > fabs(right_axis))
    {
        int isNegative = left_axis/fabs(left_axis);
        updateSingleWheelSpeed(0, left_axis);
        updateSingleWheelSpeed(1, left_axis);
        updateSingleWheelSpeed(2, left_axis);
        updateSingleWheelSpeed(3, (left_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative));
        updateSingleWheelSpeed(4, (left_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative));
        updateSingleWheelSpeed(5, (left_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative));
    }
    else if (fabs(left_axis)<fabs(right_axis))
    {
        int isNegative = right_axis/fabs(right_axis);
        updateSingleWheelSpeed(0, (right_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative));
        updateSingleWheelSpeed(1, (right_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative));
        updateSingleWheelSpeed(2, (right_axis - MAX_DIFFERENCE_PERCENT/PERCENT_MAX * isNegative));
        updateSingleWheelSpeed(3, right_axis);
        updateSingleWheelSpeed(4, right_axis);
        updateSingleWheelSpeed(5, right_axis);
    }
}

void DriveBase::updateSingleWheelSpeed(int wheelIndex, float targetSpeed)
{
    m_wheels[wheelIndex].setSpeed(targetSpeed);
}

#endif

void DriveBase::forceStop()
{
    for(int i = 0; i < NUM_WHEELS; i++)
    {
        m_wheels[i].forceStop();
    }
}