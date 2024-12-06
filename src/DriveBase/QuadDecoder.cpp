#include "../../include/QuadDecoder.h"
#include <functional>


enum ENCODER_PULSE_STATES_BITS
{
  PREV_PIN_A      = 0b00000001,
  PREV_PIN_B      = 0b00000010,
  CURR_PIN_A      = 0b00000100,
  CURR_PIN_B      = 0b00001000,
  COUNT_DIR      = 0b00010000, // 0 positive, 1 negitive
  COUNT_MAG      = 0b01100000, // if on count was increased by 2
  NULL_BIT       = 0b10000000 
};

static encoderDataStruct encoderData[ENCODER_NUM];

static void update(uint8_t wheel_num)
{
  // first shift the old data out
  encoderData[wheel_num].pulseStates = encoderData[wheel_num].pulseStates >> 2;

  // clear out all bits above 2
  encoderData[wheel_num].pulseStates &= 0b00000011;

  // enter the new pin states
  encoderData[wheel_num].pulseStates |= (ENCODER_PULSE_STATES_BITS::CURR_PIN_A * digitalReadFast(ENC_A_PINS[wheel_num])); 
  encoderData[wheel_num].pulseStates |= (ENCODER_PULSE_STATES_BITS::CURR_PIN_B * digitalReadFast(ENC_B_PINS[wheel_num])); 

  // enter the time
  encoderData[wheel_num].pulseTimes[encoderData[wheel_num].currentHead] = micros();
  encoderData[wheel_num].currentHead = !encoderData[wheel_num].currentHead;

//                           _______         _______       
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

		//	new	new	old	old
		//	pin2	pin1	pin2	pin1	Result
		//	----	----	----	----	------
		//	0	0	0	0	no movement
		//	0	0	0	1	+1
		//	0	0	1	0	-1
		//	0	0	1	1	+2  (assume pin1 edges only)
		//	0	1	0	0	-1
		//	0	1	0	1	no movement
		//	0	1	1	0	-2  (assume pin1 edges only)
		//	0	1	1	1	+1
		//	1	0	0	0	+1
		//	1	0	0	1	-2  (assume pin1 edges only)
		//	1	0	1	0	no movement
		//	1	0	1	1	-1
		//	1	1	0	0	+2  (assume pin1 edges only)
		//	1	1	0	1	-1
		//	1	1	1	0	+1
		//	1	1	1	1	no movement

  switch (encoderData[wheel_num].pulseStates) {
    case 1: case 7: case 8: case 14:
      encoderData[wheel_num].count++;
      encoderData[wheel_num].pulseStates |= 0x00100000;
      return;
    case 2: case 4: case 11: case 13:
      encoderData[wheel_num].count--;
      encoderData[wheel_num].pulseStates |= 0x00110000;
      return;
    case 3: case 12:
      encoderData[wheel_num].count += 2;
      encoderData[wheel_num].pulseStates |= 0x01010000;
      return;
    case 6: case 9:
      encoderData[wheel_num].count -= 2;
      encoderData[wheel_num].pulseStates |= 0x01010000;
      return;
  }
}

static void update0(){update(0);}
static void update1(){update(1);}
static void update2(){update(2);}
static void update3(){update(3);}
static void update4(){update(4);}
static void update5(){update(5);}

#if ENABLE_CUSTOM_ENCODER

QuadratureDecoder::QuadratureDecoder(uint8_t wheel_num)
{
  // set up local data
  this->m_wheel_num = wheel_num;
  
  this->m_current_data = encoderData[wheel_num];

  //empty the saved data
  m_current_data.pulseStates = 0;

  for(int i = 0; i<ENCODER_PULSE_SAVE_SIZE; i++)
  {
    m_current_data.pulseTimes[i] = 0;
  }

  // setup hardware
  pinMode(ENC_A_PINS[m_wheel_num], INPUT_PULLUP);
  pinMode(ENC_B_PINS[m_wheel_num], INPUT_PULLUP);

  switch (m_wheel_num)
  {
  case 0:
    attachInterrupt(ENC_A_PINS[m_wheel_num], update0, CHANGE);
    attachInterrupt(ENC_B_PINS[m_wheel_num], update0, CHANGE);
    break;
  
  case 1:
    attachInterrupt(ENC_A_PINS[m_wheel_num], update1, CHANGE);
    attachInterrupt(ENC_B_PINS[m_wheel_num], update1, CHANGE);
    break;

  case 2:
    attachInterrupt(ENC_A_PINS[m_wheel_num], update2, CHANGE);
    attachInterrupt(ENC_B_PINS[m_wheel_num], update2, CHANGE);
    break;

  case 3:
    attachInterrupt(ENC_A_PINS[m_wheel_num], update3, CHANGE);
    attachInterrupt(ENC_B_PINS[m_wheel_num], update3, CHANGE);
    break;

  case 4:
    attachInterrupt(ENC_A_PINS[m_wheel_num], update4, CHANGE);
    attachInterrupt(ENC_B_PINS[m_wheel_num], update4, CHANGE);
    break;

  case 5:
    attachInterrupt(ENC_A_PINS[m_wheel_num], update5, CHANGE);
    attachInterrupt(ENC_B_PINS[m_wheel_num], update5, CHANGE);
    break;

  default:
    break;
  }

}
  
void QuadratureDecoder::begin( void )
{
  resetCount();
}

// get the time between the last 2 pulses then get count change then bam velocity
float QuadratureDecoder::getRPM( int time_interval_ms )
{
  int cur_time_us = micros();

  noInterrupts()  
  // get the time between pulses, need abs if over 79 minutes
  int time_between_pulse_us = abs(m_current_data.pulseTimes[1] - m_current_data.pulseTimes[0]);
  
  // get the time from last pulse, need abs if over 79 minutes
  int time_from_last_pulse_us = abs(cur_time_us - m_current_data.pulseTimes[1]);

  float rpm;

  // if time from last pulse is greater than cycle time assume the wheel is not moving
  if(time_from_last_pulse_us )
  {
    interrupts();
    return 0;
  }

  // if from last pulse is greater then time_between_pulse 
  // use that time for computation of velocity instead.
  if(time_between_pulse_us < time_from_last_pulse_us)
  {
    time_between_pulse_us = time_from_last_pulse_us;
  }

  // negitive direction
  if(m_current_data.pulseStates & ENCODER_PULSE_STATES_BITS::COUNT_DIR)
  {
    rpm = -((float)((m_current_data.pulseStates & ENCODER_PULSE_STATES_BITS::COUNT_MAG) >> 5)/(float)COUNTS_PER_REV)/US_TO_MIN(time_between_pulse_us);
  }

  // positive direction
  else
  {
    rpm = ((float)((m_current_data.pulseStates & ENCODER_PULSE_STATES_BITS::COUNT_MAG) >> 5)/(float)COUNTS_PER_REV)/US_TO_MIN(time_between_pulse_us);
  }
  interrupts();
  return rpm;
}

long QuadratureDecoder::getCount( void )
{
  return m_current_data.count;
}

void QuadratureDecoder::resetCount( void )
{
  m_current_data.count = 0;
}



#else
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
  if(m_currentHead >= ENCODER_COUNTS_SAVE_SIZE)
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

#endif