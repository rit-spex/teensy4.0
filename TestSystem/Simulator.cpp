#include "../TestSystem/Simulator.h"


// define global class
//////////////////////////////////////////// Serial_Class /////////////////////////
void Serial_Class::begin(int baudrate)
{
    std::cout << "Serial begin called with baudrate: " << baudrate << std::endl;
}
//print statements
void Serial_Class::print(const char* message)
{
    std::cout << message;
}
void Serial_Class::print(const int message)
{
    std::cout << message;
}
void Serial_Class::print(const float message)
{
    std::cout << message;
}

//println statements
void Serial_Class::println(const char* message)
{
    std::cout << message << std::endl;
}
void Serial_Class::println(const float message)
{
    std::cout << message << std::endl;
}
void Serial_Class::println(const int message)
{
    std::cout << message << std::endl;
}
void Serial_Class::println(std::string message)
{
    std::cout << "Serial println called with message: " << message << std::endl;
}

////////////////////////////////////////////// Serial2_Class ////////////////////////////////

Serial2_Class::Serial2_Class()
{
    demoData[0]  = 0xde;
    demoData[1]  = 100;
    demoData[2]  = 100;
    demoData[3]  = 0x55;
    demoData[4]  = 0x55;
    currentHead  = -1;
    currentValue = 100;
    increaseDir  = 5;
}
void Serial2_Class::begin(int baudrate, int config)
{
    std::cout << "Serial2 begin called with baudrate: " << baudrate << " and config: " << config << std::endl;
}
int Serial2_Class::available()
{
    std::cout << "Serial2 available called with " << 5 <<std::endl;
    return 5;
}
int Serial2_Class::read()
{
    currentHead++;
    if(currentHead >= 5)
    {
        // make the values slowly go up then down to test values
        currentHead = 0;
        currentValue+= increaseDir;
        if(currentValue >= 200)
        {
            increaseDir = -5;
        }
        else if(currentValue <= 0)
        {
            increaseDir = 5;
        }
        demoData[1] = currentValue;
        //demoData[2] = currentValue;
    }
    std::cout << "Serial2 read called with value " <<demoData[currentHead]<< std::endl;
    return demoData[currentHead];
}

////////////////////////////////////////////// Wire_Class   ////////////////////////////////

void Wire_Class::begin()
{
    std::cout << "Wire begin called" << std::endl;
}
void Wire_Class::setSCL(int scl_pin)
{
    std::cout << "Wire setSCL called with pin: " << scl_pin << std::endl;
}
void Wire_Class::setSDA(int sda_pin)
{
    std::cout << "Wire setSDA called with pin: " << sda_pin << std::endl;
}
void Wire_Class::beginTransmission(int address)
{
    std::cout << "Wire beginTransmission called with address: " << address << std::endl;
}
void Wire_Class::write(int data)
{
    std::cout << "Wire write called with data: " << data << std::endl;
}
void Wire_Class::endTransmission()
{
    std::cout << "Wire endTransmission called" << std::endl;
}

////////////////////////////////////////////// TimerThree ////////////////////////////////

void TimerThree::initialize(int time)
{
    std::cout << "TimerThree initialize called with time: " << time << std::endl;
}
void TimerThree::start()
{
    std::cout << "TimerThree start called" << std::endl;
}
void TimerThree::stop()
{
    std::cout << "TimerThree stop called" << std::endl;
}
void TimerThree::pwm(int pin, int pwm)
{
    std::cout << "TimerThree pwm called" << std::endl;
}

////////////////////////////////////////////// Servo ////////////////////////////////

void Servo::attach(int pin, int min, int max)
{
    this->pin = pin;
    std::cout << "Servo attach called with pin: " << pin << " min: "<< min << " max: "<< max << std::endl;
}
void Servo::detach()
{
    std::cout << "Servo detach called on pin: " << pin << std::endl;
}
void Servo::writeMicroseconds(int microseconds)
{
    std::cout << "Servo write called with microseconds: " << microseconds << " on pin" << pin << std::endl;
}

////////////////////////////////////////////// TicI2C ////////////////////////////////

TicI2C::TicI2C(int address)
{
    std::cout << "TicI2C constructor called with address: " << address << std::endl;
}
void TicI2C::exitSafeStart()
{
    std::cout << "TicI2C exitSafeStart called" << std::endl;
}
void TicI2C::setTargetVelocity(int velocity)
{
    std::cout << "TicI2C setTargetVelocity called with velocity: " << velocity << std::endl;
}
void TicI2C::haltAndHold()
{
    std::cout << "TicI2C haltAndHold called" << std::endl;
}

////////////////////////////////////////////// Encoder ////////////////////////////////

Encoder::Encoder(int pin1, int pin2)
{
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->counts = 1000;
    std::cout << "Encoder constructor called" << std::endl;
}
void Encoder::attach(int pin1, int pin2)
{
    this->pin1 = pin1;
    this->pin2 = pin2;
    std::cout << "Encoder attach called with pin1: " << pin1 << " and pin2: " << pin2 << std::endl;
}
long Encoder::read()
{
    //std::cout << "Encoder read called on encoder with pins: "<< pin1 << ", " << pin2 << std::endl;
    return this->counts;
}
void Encoder::write(int value)
{
    //std::cout << "Encoder write called with value: " << value << std::endl;
}

////////////////////////////////////////////// FlexCAN_T4 ////////////////////////////////

FlexCAN_T4::FlexCAN_T4()
{
    std::cout << "FlexCAN_T4 constructor called" << std::endl;
}
void FlexCAN_T4::begin()
{
    std::cout << "FlexCAN_T4 begin called" << std::endl;
}
void FlexCAN_T4::write(FLEXCAN_MAILBOX mailbox ,const CAN_message_t &msg)
{
    std::cout << "FlexCAN_T4 write called with id: " << msg.id << std::endl;
}
void FlexCAN_T4::setMB(uint8_t mailbox, int direction, FLEXCAN_IDE ide)
{
    std::cout << "FlexCAN_T4 setMB called with mailbox: " << mailbox << std::endl;
}
void FlexCAN_T4::enableMBInterrupt(FLEXCAN_MAILBOX mailbox)
{
    std::cout << "FlexCAN_T4 enableMBInterrupt called with mailbox: " << mailbox << std::endl;
}
void FlexCAN_T4::setBaudRate(int baudrate)
{
    std::cout << "FlexCAN_T4 setBaudRate called with baudrate: " << baudrate << std::endl;
}
void FlexCAN_T4::onReceive(FLEXCAN_MAILBOX mailbox ,void (*callback)(const CAN_message_t &msg))
{
    std::cout << "FlexCAN_T4 onReceive called" << std::endl;
}
std::string FlexCAN_T4::events()
{
    std::cout<< "FlexCAN_T4 events called" << std::endl;
    return "FlexCAN_T4 events";
}

// define global functions
void pinMode(int pin, int mode)
{
    std::cout << "pinMode called with pin: " << pin << " and mode: " << mode << std::endl;
}

void digitalWrite(int pin, int value)
{
    std::cout << "digitalWrite called with pin: " << pin << " and value: " << value << std::endl;
}

void delay(int milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    std::cout << "delay called with milliseconds: " << milliseconds << std::endl;
}

unsigned long millis()
{
    static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    unsigned long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    //std::cout << "millis called at " << time << std::endl;
    return time;
}

void analogWrite(int pin, int pwm)
{
    std::cout << "analogWrite called" << std::endl;
}

float analogRead(int pin)
{
    std::cout << "analogRead called" << std::endl;
    return 0;
}
