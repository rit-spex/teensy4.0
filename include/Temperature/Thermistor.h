/*
File: thermistor.h
Author: Ryan Barry
Date Created: 1/23/2024

This file defines the thermistor class for the rover.
This class is responsible for reading the temperature of a thermistor.
*/

#ifndef THERMISTOR_H
#define THERMISTOR_H

#include "DEBUG.h"
#include "Pinout.h"
#include <math.h>

#if ENABLE_SIMULATOR
#include "../TestSystem/Simulator.h"
#else
#include <Wire.h>
#include <Arduino.h>
#endif

class Thermistor {
    public:
        /*
        * Constructor for the thermistor class.
        * Initializes the thermistor.
        */
        Thermistor(uint8_t thermistor_id);

        /*
        * @return The temperature measured by the thermistor
        */
        float getTemperature();
    private:
        // the id number of the thermistor, matches with fan_id, used to get pin
        uint8_t m_thermistor_id;

        // the current temperature
        float m_temperature;
};

#endif
