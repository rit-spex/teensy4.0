#include "../../include/DriveBase.h"

/*
* Constructor for the drive base class.
* Initializes the wheels of the rover.
*/
DriveBase::DriveBase():
wheel1(PWM_PIN_0),
wheel2(PWM_PIN_1),
wheel3(PWM_PIN_2),
wheel4(PWM_PIN_3),
wheel5(PWM_PIN_4),
wheel6(PWM_PIN_5)
{
    // this->wheel1 = Wheel(PWM_PIN_0);//, ENC_A_PIN_0, ENC_B_PIN_0, PIDConstants::KP0, PIDConstants::KI0, PIDConstants::KD0);
    // this->wheel2 = Wheel(PWM_PIN_1);//, ENC_A_PIN_1, ENC_B_PIN_1, PIDConstants::KP1, PIDConstants::KI1, PIDConstants::KD1);
    // this->wheel3 = Wheel(PWM_PIN_2);//, ENC_A_PIN_2, ENC_B_PIN_2, PIDConstants::KP2, PIDConstants::KI2, PIDConstants::KD2);
    // this->wheel4 = Wheel(PWM_PIN_3);//, ENC_A_PIN_3, ENC_B_PIN_3, PIDConstants::KP3, PIDConstants::KI3, PIDConstants::KD3);
    // this->wheel5 = Wheel(PWM_PIN_4);//, ENC_A_PIN_4, ENC_B_PIN_4, PIDConstants::KP4, PIDConstants::KI4, PIDConstants::KD4);
    // this->wheel6 = Wheel(PWM_PIN_5);//, ENC_A_PIN_5, ENC_B_PIN_5, PIDConstants::KP5, PIDConstants::KI5, PIDConstants::KD5);
    // this->wheels = {
    //     Wheel(PWM_PIN_0, ENC_A_PIN_0, ENC_B_PIN_0, PIDConstants::KP0, PIDConstants::KI0, PIDConstants::KD0),
    //     Wheel(PWM_PIN_1, ENC_A_PIN_1, ENC_B_PIN_1, PIDConstants::KP1, PIDConstants::KI1, PIDConstants::KD1),
    //     Wheel(PWM_PIN_2, ENC_A_PIN_2, ENC_B_PIN_2, PIDConstants::KP2, PIDConstants::KI2, PIDConstants::KD2),
    //     Wheel(PWM_PIN_3, ENC_A_PIN_3, ENC_B_PIN_3, PIDConstants::KP3, PIDConstants::KI3, PIDConstants::KD3),
    //     Wheel(PWM_PIN_4, ENC_A_PIN_4, ENC_B_PIN_4, PIDConstants::KP4, PIDConstants::KI4, PIDConstants::KD4),
    //     Wheel(PWM_PIN_5, ENC_A_PIN_5, ENC_B_PIN_5, PIDConstants::KP5, PIDConstants::KI5, PIDConstants::KD5)};
    this->targetVelocity[0] = 0;
    this->targetVelocity[1] = 0;
    this->targetVelocity[2] = 0;
    this->targetVelocity[3] = 0;
    this->targetVelocity[4] = 0;
    this->targetVelocity[5] = 0;
}

// Retrieves the target velocity from the CAN bus
void DriveBase::getTargetVelocity()
{
    // if(m_can->newMessage(CAN::Message_ID::TARGET_VELOCITY))
    // {
    //     CAN_message_t msg = m_can->getMessage(CAN::Message_ID::TARGET_VELOCITY);
    //     for (int i = 0; i < NUM_WHEELS; i++) 
    //     {
    //         if((msg.buf[6]&(1<<i)) != 0)//if bit is on
    //         {
    //             targetVelocity[i] = (float)msg.buf[i] * -1; // This line will change based on message packing
    //         }
    //         else
    //         {
    //             targetVelocity[i] = (float)msg.buf[i] * 1; // This line will change based on message packing
    //         }
    //     }
    // }
}

void DriveBase::drive(float left_axis, float right_axis)
{
    //Serial.println(fabs(left_axis - right_axis));
    //Serial.println(0.14/PERCENT_MAX);
    if (fabs(fabs(left_axis) - fabs(right_axis)) < (0.14/PERCENT_MAX))
    {
        updateSingleWheel(0, left_axis);
        updateSingleWheel(1, left_axis);
        updateSingleWheel(2, left_axis);
        updateSingleWheel(3, right_axis);
        updateSingleWheel(4, right_axis);
        updateSingleWheel(5, right_axis);   
    }
    else if (fabs(left_axis) > fabs(right_axis))
    {
        int isNegative = left_axis/fabs(left_axis);
        updateSingleWheel(0, left_axis);
        updateSingleWheel(1, left_axis);
        updateSingleWheel(2, left_axis);
        updateSingleWheel(3, -(left_axis - 0.14/PERCENT_MAX * isNegative));
        updateSingleWheel(4, -(left_axis - 0.14/PERCENT_MAX * isNegative));
        updateSingleWheel(5, -(left_axis - 0.14/PERCENT_MAX * isNegative));   
    }
    else if (fabs(left_axis)<fabs(right_axis))
    {
        int isNegative = right_axis/fabs(right_axis);
        updateSingleWheel(0, -(right_axis - 0.14/PERCENT_MAX * isNegative));
        updateSingleWheel(1, -(right_axis - 0.14/PERCENT_MAX * isNegative));
        updateSingleWheel(2, -(right_axis - 0.14/PERCENT_MAX * isNegative));
        updateSingleWheel(3, right_axis);
        updateSingleWheel(4, right_axis);
        updateSingleWheel(5, right_axis);   
    }
}

// Updates the velocity of the wheels to match the target velocity
void DriveBase::updateVelocity()
{
    getTargetVelocity();
    this->wheel1.setSpeed(targetVelocity[0]);
    this->wheel2.setSpeed(targetVelocity[1]);
    this->wheel3.setSpeed(targetVelocity[2]);
    this->wheel4.setSpeed(targetVelocity[3]);
    this->wheel5.setSpeed(targetVelocity[4]);
    this->wheel6.setSpeed(targetVelocity[5]);

    // for (int i = 0; i < NUM_WHEELS; i++) {
    //     wheels[i].setSpeed(targetVelocity[i]);
    // }
}

void DriveBase::updateSingleWheel(int wheelIndex, float targetVelocity)
{
    switch (wheelIndex)
    {
    case 
        0:
        this->wheel1.setSpeed(targetVelocity);
        break;
    case 1: 
        this->wheel2.setSpeed(targetVelocity);
        break;
    case 2:
        this->wheel3.setSpeed(targetVelocity);
        break;
    case 3:
        this->wheel4.setSpeed(targetVelocity);
        break;
    case 4:
        this->wheel5.setSpeed(targetVelocity);
        break;
    case 5:
        this->wheel6.setSpeed(targetVelocity);
        break;
    default:
        break;
    }
    //this->wheels[wheelIndex].setSpeed(targetVelocity);
}
