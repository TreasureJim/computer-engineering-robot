#include "irsensor.h"

void initialize_sensors() {
    // Set the DDR as input for port C (analog pins)
    DDRC |= (0 << right_sensor) | (0 << left_sensor) | (0 << mid_sensor);
    //REFS0 - External Reference voltage (+5V)
    //ADLAR - Left shift the resulting ADC register (Only ADCH can be read)
    ADMUX |= (1 << REFS0) | (1 << ADLAR);
    //ADEN - Enable ADC
    //ADSC - Start the first conversion
    //ADPS2:0 - Prescalar bits, set to 128
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

