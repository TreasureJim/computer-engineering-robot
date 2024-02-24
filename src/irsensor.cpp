#include "irsensor.h"
#include "pins.h"
#include "motor.h"
/// @brief Sensor initialization for the DDR and ADC.
void initialize_sensors() {
    // Set the DDR as input for port C (analog pins)
    DDRC |= (0 << rightMidSensor) | (0 << leftMidSensor) | (0 << midMidSensor) | (0 << leftSensor) | (0 << rightSensor);
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

/// @brief Calculates the minimum and maximum value of the sensors
/// This function does not return any values. Instead, the pointers
/// passed as parameters will be set outside this function.
/// @param min pointer to the min variable
/// @param max pointer to the max variable
void getLimits(uint8_t *min, uint8_t *max) {
    uint8_t localMin = 255;
    uint8_t localMax = 0;
    for(int i = 0; i <= 1000; i++) {
        uint8_t val = getAverageValue();
        _delay_ms(10);
        if (val < localMin) {
            localMin = val;
        } else if (val > localMax) {
            localMax = val;
        }
    }
    *min = localMin;
    *max = localMax;
}

///@brief Calculates the average value of the readings of the IR sensors
///@return Average value of all sensors
uint8_t getAverageValue() {
    uint8_t v1 = analogread(rightMidSensor);
    uint8_t v2 = analogread(leftMidSensor);
    uint8_t v3 = analogread(midMidSensor);
    uint8_t v4 = analogread(leftSensor);
    uint8_t v5 = analogread(rightSensor);
    return ((v1 + v2 + v3 + v4 + v5) / 5);
}

/// @brief Calculates a scaled value of the average value of all sensors.
/// getAverageValue() is called to get the values from the sensors and is
/// then scaled to a value between 0 and 1.
/// @param min min value of the average
/// @param max max value of the average
/// @return A float between 0 and 1 based on the given min and max values.
float getScaledValue(uint8_t *min, uint8_t *max) {
    uint8_t avg = getAverageValue();
    return ((float)(avg - *min) / (float)(*max - *min));
}

void calibrateSensors(uint8_t *min, uint8_t *max) {
    start_motors();
    _delay_ms(2000);
    drive_motors(1);
    uint8_t localMin;
    uint8_t localMax;
    getLimits(&localMin, &localMax);
    *min = localMin;
    *max = localMax;
    cut_motors();
}