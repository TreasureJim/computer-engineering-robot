#include "irsensor.h"

/// @brief Sensor initialization for the DDR and ADC.
void initialize_sensors() {
    // Set the DDR as input for port C (analog pins)
    DDRC |= (0 << rightMidSensor) | (0 << leftMidSensor) | (0 << midMidSensor);
    //REFS0 - External Reference voltage (+5V)
    //ADLAR - Left shift the resulting ADC register (Only ADCH can be read)
    ADMUX |= (1 << REFS0) | (1 << ADLAR);
    //ADEN - Enable ADC
    //ADPS2:0 - Prescalar bits, set to 128
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/// @brief Reads the analog value on the analog pins
/// @param channel 0-4
/// @return Value of the ADCH register
uint8_t analogread(uint8_t channel) {
    //Unsets the channel bits from previous call and preserve the upper 4 bits of the ADMUX register
    //0x60 Keeps the REFS0 and ADLAR bits set (0110)
    ADMUX = (ADMUX & 0x60) | (channel & 0x0F);
    //Start ADC conversion
    ADCSRA |= (1 << ADSC);
    //Wait for the conversion to be done
    while(ADCSRA & (1 << ADSC));;
    return ADCH;
}

/// @brief Calculates the minimum and maximum value of the
/// average value taken from the 3 middle IR sensors.
/// This function does not return any values. Instead, the pointers
/// passed as parameters will be set outside this function.
/// @param min pointer to the min variable
/// @param max pointer to the max variable
void getLimits(uint8_t *min, uint8_t *max) {
    uint8_t localmin = 255;
    uint8_t localmax = 0;
    for(int i = 0; i <= 1000; i++) {
        uint8_t avg = getAverageValue();
        _delay_ms(10);
        if (avg < localmin) {
            localmin = avg;
        } else if (avg > localmax) {
            localmax = avg;
        }
    }
    *min = localmin;
    *max = localmax;
}

///@brief Gets the average value of the readings of the IR sensors
///@return Average value of the 3 middle sensors
uint8_t getAverageValue() {
    uint8_t v1 = analogread(rightMidSensor);
    uint8_t v2 = analogread(leftMidSensor);
    uint8_t v3 = analogread(midMidSensor);
    return ((v1 + v2 + v3) / 3);
}

/// @brief This function calculates a scaled value between 0 and 1.
/// @param val Value given by the IR sensor
/// @param min min value of the value parameter
/// @param max max value of the value parameter
/// @return A float between 0 and 1 based on the parameters passed
float getScaledValue(uint8_t value, uint8_t *min, uint8_t *max) {
    return ((float)(value - *min) / (float)(*max - *min));
}

// This file needs to be reformatted if the scaling for the additional 2 sensors isn't working properly
// Either reformatted or some code is going to be duplicated to account for the additional 2 sensors
// Some functions are made entirely to accomodate the 3 middle sensors and calculating based on the average
// value that was received such as getLimits().
// For testing, the additional sensors must be connected to the on-board arduino and the connections must be fixed
// Otherwise the test values are inaccurate.