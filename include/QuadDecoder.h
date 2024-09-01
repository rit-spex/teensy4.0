#ifndef QUADDECODER_H
#define QUADDECODER_H

#include "Pinout.h"
#include "Constants.h"
#include "DEBUG.h"

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Encoder.h>
#include <IntervalTimer.h>
#endif
#define ENCODER_SAVE_SIZE 3

class QuadratureDecoder {
public:
  QuadratureDecoder(ENC_A_PINS enc_A_pin, ENC_B_PINS enc_B_pin);
//  void begin();
//  long getCount();
  float getRPM(int time_interval_ms);

private:
  // the physical encoder
  Encoder m_encoder;
  int enc_A_pin;
  int enc_B_pin;

  // the last x counts from the encoder
  long m_count[ENCODER_SAVE_SIZE];

  // the last x time interval from the encoder
  long m_timeInterval[ENCODER_SAVE_SIZE];

  // the currentHead of the save data
  int m_currentHead;

  // enter the encoder data to the save data
  void updateCount(int time_interval_ms);

  // average count
  float getAverageCount();
  // average time
  float getAverageTime();
};

#endif
