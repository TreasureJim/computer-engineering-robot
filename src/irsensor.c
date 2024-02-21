#include "irsensor.h"

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

uint8_t analogread(uint8_t channel) {
    //Unsets the channel bits from previous call and preserve the upper 4 bits of the ADMUX register
    //0x60 Keeps the REFS0 and ADLAR bits set (0110)
    ADMUX = (ADMUX & 0x60) | (channel & 0x0F);
    //Start ADC conversion
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));;
    return ADCH;
}

void readIRsensors(uint8_t* sensorValues) {
    sensorValues[0] = analogread(right_sensor);
    sensorValues[1] = analogread(left_sensor);
    sensorValues[2] = analogread(mid_sensor);
}