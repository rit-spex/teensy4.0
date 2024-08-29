#ifndef QUADDECODER_H
#define QUADDECODER_H

#include "Pinout.h"
#include <Encoder.h>
#include <IntervalTimer.h>
#include "Constants.h"

class QuadratureDecoder {
public:
  QuadratureDecoder(ENC_A_PINS enc_A_pin, ENC_B_PINS enc_B_pin);
//  void begin();
  long getCount();
  float getRPM(int time_interval_ms);

private:
  Encoder m_encoder;
  volatile long m_count;
  int enc_A_pin;
  int enc_B_pin;
  static QuadratureDecoder* instance;
  static void updateCount();
};

#endif