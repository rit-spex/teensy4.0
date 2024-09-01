// classes to simulate the arduino functions
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdint.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

class Serial_Class
{
    public:

        void begin(int baudrate);

        //print statements
        void print(const char* message);
        void print(const int message);
        void print(const float message);

        //println statements
        void println(const char* message);
        void println(const float message);
        void println(const int message);
        void println(std::string message);

};

class Serial2_Class
{
    private:
        int demoData[5];
        int currentHead;
        int currentValue;
        int increaseDir;
    public:
        Serial2_Class();
        void begin(int baudrate, int config);
        int available();
        int read();
};

class Wire_Class
{
    public:
        void begin();
        void setSCL(int scl_pin);
        void setSDA(int sda_pin);
        void beginTransmission(int address);
        void write(int data);
        void endTransmission();
};

class TimerThree
{
    public:
        void initialize(int time);
        void start();
        void stop();
        void pwm(int pin, int pwm);
};

class Servo
{
    private:
        int pin;
    public:
        void attach(int pin, int min, int max);
        void detach();
        void writeMicroseconds(int microseconds);
};

class TicI2C
{
    public:
        TicI2C(int address);
        void exitSafeStart();
        void setTargetVelocity(int velocity);
        void haltAndHold();
};

class Encoder
{
    private:
        int pin1;
        int pin2;
        int counts;
    public:
        Encoder(int pin1, int pin2);
        void attach(int pin1, int pin2);
        long read();
        void write(int value);
};

struct CAN_message_t
{
    uint32_t id;
    uint8_t buf[8];
    uint8_t len;
};

enum FLEXCAN_MAILBOX
{
    MB0 = 0,
    MB1 = 1,
    MB2 = 2,
    MB3 = 3
};

enum FLEXCAN_IDE
{
    STD = 0
};

class FlexCAN_T4
{
    public:
        FlexCAN_T4();
        void begin();
        void write(FLEXCAN_MAILBOX mailbox ,const CAN_message_t &msg);
        void setMB(uint8_t mailbox, int direction, FLEXCAN_IDE ide);
        void enableMBInterrupt(FLEXCAN_MAILBOX mailbox);
        void setBaudRate(int baudrate);
        void onReceive(FLEXCAN_MAILBOX mailbox ,void (*callback)(const CAN_message_t &msg));
        std::string events();
};

static Serial_Class  Serial;
static Serial2_Class Serial2;
static Wire_Class    Wire;
static TimerThree    Timer3;

#define SERIAL_8N1 0

#define INPUT 0
#define OUTPUT 1

#define LOW 0
#define HIGH 1

#define RX 0
#define TX 1

// define global functions
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
void delay(int milliseconds);
unsigned long millis();
void analogWrite(int pin, int pwm);
float analogRead(int pin);

#endif
