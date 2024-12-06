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
#include <Arduino.h>
#endif

typedef struct encoderDataStruct
{
    // the head of the data
    volatile uint8_t currentHead;

    // see PULSE_STATE_BITS
    uint8_t pulseStates; 
    uint32_t pulseTimes[ENCODER_PULSE_SAVE_SIZE];
    long count;
} encoderDataStruct_t;

class QuadratureDecoder {
public:  
  void begin( void );
  float getRPM( int time_interval_ms );

  #if ENABLE_CUSTOM_ENCODER

  QuadratureDecoder(uint8_t wheel_num);

  long getCount( void );
  void resetCount( void );

  #else

  QuadratureDecoder(int wheel_num);

  // enter the encoder data to the save data
  void updateCount(int time_interval_ms);


  #endif

private:

  uint8_t m_wheel_num;

  #if ENABLE_CUSTOM_ENCODER

  encoderDataStruct_t m_current_data;

  #else

  int m_currentHead;

  // the physical encoder
  Encoder m_encoder;

  // the last x counts from the encoder
  long m_count[ENCODER_COUNTS_SAVE_SIZE];

  // the last x time interval from the encoder
  long m_timeInterval[ENCODER_COUNTS_SAVE_SIZE];

  // average count
  float getAverageCount();
  // average time
  float getAverageTime();

  #endif
};

#endif