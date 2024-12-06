#include "../include/Extra/Xbee.h"
#include <algorithm>

// unicast mode
// Configure the DL/DH of the xbee to the SL/SH of the xbee talk directly to each other
// Configure the RR (MAC retries to 0) - doesn't wait for an ACK
// RO = 0; packetization timeout to 0 - doesn't wait for an
// NP - Maximum number of bytes;

// xbee recomend 10ms delay between packets

Xbee::Xbee()
{
    //Serial2.begin(9600, SERIAL_8N1);
    //Serial2.addMemoryForRead(bigserialbuffer, sizeof(bigserialbuffer));
    Serial.println("Xbee Initialized");
    for(int i = 0; i<NUM_BUTTONS; i++)
    {
        // buttons default to false
        for(int j = 0; j<SAVE_SIZE; j++)
        {
            m_buttonvalues[i][j] = false;
        }
    }
    for (int i = 0; i < NUM_AXES; i++)
    {
        for (int j = 0; j < SAVE_SIZE; j++)
        {
            // axis values default to 0
            m_axisvalues[i][j] = 0.0;
        }
    }
    m_numNoSignal = 0;
}

void Xbee::UpdateValues()
{
    // if the xbee is disabled, don't update the values
    if(m_isDisabled)
    {
        return;
    }

    // if the xbee has not connected yet, don't start the disable timer
    if(!m_firstConnected)
    {
        // yay, the xbee is connected
        if(Serial2.available() >= 5)
        {
            m_firstConnected = true;
        }
        // no connection detected, don't update the values
        else
        {
            return;
        }
    }

    //get the values from the xbee
    if(Serial2.available() >= 5)
    {
        if(Serial2.read() != START_COMMAND)
        {
            return;
        }

        //reset disable counter
        m_numNoSignal = 0;

        //parse the current message
        parseMessage();

        //update the current median axis values
        for (int i = 0; i < NUM_AXES; i++)
        {
            float newValue = findAxisMedian(i);
            if(m_currentValues[i] != newValue)
            {
                m_currentValues[i] = newValue;
                m_isNewValuesFound = true;
            }
        }

        //update the current median buttons values
        for(int i = 0; i < NUM_BUTTONS; i++)
        {
            float newValue = findButtonMedian(i);
            if(m_currentValues[i+NUM_AXES] != newValue)
            {
                m_currentValues[i+NUM_AXES] = newValue;
                m_isNewValuesFound = true;
            }

        }

    }
    else
    {
        m_numNoSignal++;
        if(m_numNoSignal > NO_MESSAGE_ERROR_NUM)
        {
            // empty out of the values
            for(int i = 0; i<NUM_BUTTONS; i++)
            {
                for(int j = 0; j<SAVE_SIZE; j++)
                {
                    m_buttonvalues[i][j] = false;
                }
            }
            for (int i = 0; i < NUM_AXES; i++)
            {
                for (int j = 0; j < SAVE_SIZE; j++)
                {
                    m_axisvalues[i][j] = 0.0;
                }
            }
            m_isDisabled = true;
        }

    }
}

void Xbee::parseMessage()
{
    //get the value of the axes
    for(int i = 0; i < NUM_AXES; i++)
    {
        int input = Serial2.read();
        if(input > 200 || input < 0){}
        else
        {
            float value = ((input - 100.0)/100.0) * PERCENT_MAX;
            this->m_axisvalues[i][m_currentHead] = value;
        }
    }

    //get the value of the buttons a,b,x,y
    bool bin[8];
    int input = Serial2.read();
    intToBinary(input, bin);
    for (int i = 0; i < 8; i+=2)
    {
        if(bin[i] == true && bin[i+1] == false)
        {
            this->m_buttonvalues[i/2][m_currentHead] = true;
        }
        else
        {
            this->m_buttonvalues[i/2][m_currentHead] = false;
        }
    }

    // get the value of the buttons lb, rb and triggers
    input = Serial2.read();
    intToBinary(input, bin);
    for (int i = 0; i < 8; i+=2)
    {
        if(bin[i] == true && bin[i+1] == false)
        {
            this->m_buttonvalues[i/2+4][m_currentHead] = true;
        }
        else
        {
            this->m_buttonvalues[i/2+4][m_currentHead] = false;
        }
    }

    // update the head
    m_currentHead++;
    if(m_currentHead >= SAVE_SIZE)
    {
        m_currentHead=0;
    }
}

float Xbee::getCurrentValue(CONTROLLER controller)
{
    return m_currentValues[controller];
}

float Xbee::findAxisMedian(int index)
{
    int num_higher = 0;
    int num_lower = 0;
    int num_same = 0;
    //Serial.println("axis median");
    for(int i = 0; i<SAVE_SIZE; i++)
    {
        // Serial.print("axis ");
        // Serial.print(index);
        // Serial.print(": ");
        // Serial.println(this->axisvalues[index][i]);
    }
    //int n = sizeof(this->axisvalues[index])/sizeof(this->axisvalues[index][0]);
    for(int i = 0; i<SAVE_SIZE; i++)
    {
        num_lower=0;
        num_higher=0;
        num_same=0;
        for(int j = 0; j<SAVE_SIZE; j++)
        {
            if(this->m_axisvalues[index][i] < this->m_axisvalues[index][j])
            {
                num_higher++;
            }
            else if(this->m_axisvalues[index][i] > this->m_axisvalues[index][j])
            {
                num_lower++;
            }
            else
            {
                num_same++;
            }
        }
        if(abs(num_higher-num_lower) < num_same)
        {
            return this->m_axisvalues[index][i];
        }
    }
    return 0;
}

bool Xbee::findButtonMedian(int index)
{
    int num_true = 0;
    int num_false = 0;
    for(int i = 0; i<SAVE_SIZE; i++)
    {
        if(this->m_buttonvalues[index][i])
        {
            num_true++;
        }
        else
        {
            num_false++;
        }
    }
    return num_true>num_false;
}

void Xbee::intToBinary(int n, bool *bin)
{
    //bool bin[8];
    for(int i = 7; i>=0; i--)
    {
        bin[i] = n%2;
        n = n/2;
    }
}

bool Xbee::isDisabled()
{
    return m_isDisabled;
}

bool Xbee::isNewValues()
{
    // return if new values are found and reset the flag
    bool temp = m_isNewValuesFound;
    m_isNewValuesFound = false;
    return temp;
}

void Xbee::printValues()
{
    #if ENABLE_SERIAL
    Serial.print("Left Axis: ");
    Serial.print(getCurrentValue(CONTROLLER::LEFT_Y_AXIS));
    Serial.print(" Right Axis: ");
    Serial.print(getCurrentValue(CONTROLLER::RIGHT_Y_AXIS));
    Serial.print(" A Button: ");
    Serial.print(getCurrentValue(CONTROLLER::A_BUTTON));
    Serial.print(" B Button: ");
    Serial.print(getCurrentValue(CONTROLLER::B_BUTTON));
    Serial.print(" X Button: ");
    Serial.print(getCurrentValue(CONTROLLER::X_BUTTON));
    Serial.print(" Y Button: ");
    Serial.print(getCurrentValue(CONTROLLER::Y_BUTTON));
    Serial.print(" LB Button: ");
    Serial.print(getCurrentValue(CONTROLLER::LB_BUTTON));
    Serial.print(" RB Button: ");
    Serial.print(getCurrentValue(CONTROLLER::RB_BUTTON));
    Serial.print(" LT Button: ");
    Serial.print(getCurrentValue(CONTROLLER::LEFT_TRIGGER));
    Serial.print(" RT Button: ");
    Serial.println(getCurrentValue(CONTROLLER::RIGHT_TRIGGER));
    #endif
}


