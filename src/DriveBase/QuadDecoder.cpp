#include "../../include/DriveBase/QuadDecoder.h"

QuadratureDecoder::QuadratureDecoder(ENC_A_PINS enc_A_pin, ENC_B_PINS enc_B_pin)
 :m_encoder(enc_A_pin, enc_B_pin), enc_A_pin(enc_A_pin), enc_B_pin(enc_B_pin)
{
    // set the current head to the start
    m_currentHead = 0;

    //empty the saved data
    for(int i = 0; i<ENCODER_SAVE_SIZE; i++)
    {
        m_count[i] = 0;
        m_timeInterval[i] = 0;
    }
}

//IS THIS NEEDED?
void QuadratureDecoder::begin() 
{
  m_encoder.write(0);
}

// get the current count of the encoder. Should only be used after updateCount() is called
//long QuadratureDecoder::getCount() {
//  return this->m_count;
//}

// update the count of the encoder
void QuadratureDecoder::updateCount(int time_interval_ms)
{
  //enter the current count and time into the save data
  this->m_count[m_currentHead] = this->m_encoder.read();
  this->m_timeInterval[m_currentHead] = time_interval_ms;

  // reset the encoder count to 0
  this->m_encoder.write(0);

  m_currentHead++;
  if(m_currentHead >= ENCODER_SAVE_SIZE)
  {
      m_currentHead = 0;
  }
}

// get the RPM from the encoder
float QuadratureDecoder::getRPM(int time_interval_ms)
{
  // // get the current count of the encoder and reset the count
  updateCount(time_interval_ms);

  // calculate the RPM
  //RPM = rotations/min = (count/counts_per_rotation)/(time_interval_ms/(seconds_per_minute * milliseconds_per_second))
  float rotation = (getAverageCount())/((float)COUNTS_PER_REV);
  float timeMin  = (getAverageTime())/((float)MS_IN_MIN);

  // prevent return /0
  if(timeMin == 0)
  {
    return 0;
  }

  float rpm = (rotation)/(timeMin);// /(time_interval_ms/(60 * 1000)));
  return rpm;
}

float QuadratureDecoder::getAverageTime()
{
    float total = 0;
    for(int i =0; i<ENCODER_SAVE_SIZE; i++)
    {
        total+= this->m_timeInterval[i];
    }
    return total/(float)ENCODER_SAVE_SIZE;
}

float QuadratureDecoder::getAverageCount()
{
    float total = 0;
    for(int i =0; i<ENCODER_SAVE_SIZE; i++)
    {
        total+= this->m_count[i];
    }
    return total/(float)ENCODER_SAVE_SIZE;
}

// #include "../../include/QuadDecoder.h"


// QuadratureDecoder* QuadratureDecoder::instance = nullptr;

// QuadratureDecoder::QuadratureDecoder(ENC_A_PINS enc_A_pin, ENC_B_PINS enc_B_pin) : m_encoder(enc_A_pin, enc_B_pin), m_count(0), enc_A_pin(enc_A_pin), enc_B_pin(enc_B_pin) 
// {
//   instance = this;
// }

// void QuadratureDecoder::begin() {
//   m_encoder.write(0);
//   // IntervalTimer timer;
//   // timer.begin([]() { updateCount(); }, 1000);  // Update count every 1000 microseconds
// }

// long QuadratureDecoder::getCount() 
// {
//   updateCount();
//   return instance->m_count;
// }

// void QuadratureDecoder::updateCount() {
//   instance->m_count += instance->m_encoder.read();
//   instance->m_encoder.write(0);
// }