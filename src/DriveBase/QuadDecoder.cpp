#include "../../include/QuadDecoder.h"


QuadratureDecoder* QuadratureDecoder::instance = nullptr;

QuadratureDecoder::QuadratureDecoder(ENC_A_PINS enc_A_pin, ENC_B_PINS enc_B_pin) : m_encoder(enc_A_pin, enc_B_pin), m_count(0), enc_A_pin(enc_A_pin), enc_B_pin(enc_B_pin) {
  instance = this;
}

//IS THIS NEEDED?
// void QuadratureDecoder::begin() {
//   m_encoder.write(0);
//   IntervalTimer timer;
//   timer.begin([]() { updateCount(); }, 1000);  // Update count every 1000 microseconds
// }

// get the current count of the encoder. Should only be used after updateCount() is called
long QuadratureDecoder::getCount() {
  return instance->m_count;
}

// update the count of the encoder
void QuadratureDecoder::updateCount() {
  instance->m_count += instance->m_encoder.read();
  instance->m_encoder.write(0);
}

// get the RPM from the encoder
float QuadratureDecoder::getRPM(int time_interval_ms) 
{
  // get the current count of the encoder and reset the count
  int count = instance->m_encoder.read();
  instance->m_count += count;
  instance->m_encoder.write(0);

  // calculate the RPM
  //RPM = rotations/min = (count/counts_per_rotation)/(time_interval_ms/(seconds_per_minute * milliseconds_per_second))
  float rpm = (count/COUNTS_PER_REV)/(time_interval_ms/(60 * 1000));
  return rpm;
}