#include "irsensor.h"

/// @brief Sensor initialization for the DDR and ADC.
void initialize_sensors() {
    // Set the DDR as input for port C (analog pins)
    DDRC |= (0 << right_sensor) | (0 << left_sensor) | (0 << mid_sensor);
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

void getLimits(uint8_t channel, uint8_t* min, uint8_t* max) {
    uint8_t localmin = 255;
    uint8_t localmax = 0;
    for(int i = 0; i <= 1000; i++) {
        uint8_t reading = analogread(channel);
        _delay_ms(10);
        if (reading < localmin) {
            localmin = reading;
        } else if (reading > localmax) {
            localmax = reading;
        }
    }
    *min = localmin;
    *max = localmax;
}

///@brief Gets the average value of the readings of the IR sensors
///@return Average value of the 3 middle sensors
// uint8_t getAverageValue() {
//     uint8_t v1 = analogread(right_sensor);
//     uint8_t v2 = analogread(left_sensor);
//     uint8_t v3 = analogread(mid_sensor);
//     return ((v1 + v2 + v3) / 3);
// }

float getScaledValue(uint8_t val, uint8_t *min, uint8_t *max) {
    return ((float)(val - *min) / (float)(*max - *min));
}