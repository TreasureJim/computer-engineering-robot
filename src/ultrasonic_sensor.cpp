#include "ultrasonicsensor.h"
#include <avr/io.h>

#define F_CPU 16000000UL
#define SPEED_OF_SOUND 343

volatile uint8_t elapsedtimeMS;
/// @brief Initialization for the Ultrasonic Sensor
/// This Sensor uses timer2 in CTC mode
void Initialize_UltrasonicSensor() {
    // Set ECHO to input and TRIGGER to output
    DDRB |= (0 << ECHO_PIN) | (1 << TRIGGER_PIN);
}

void sendPulse() {
    PORTB &= ~(1 << TRIGGER_PIN);
    _delay_us(2);
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIGGER_PIN);
}

uint16_t measureEcho() {
    uint16_t count = 0;
    while(!(PINB & (1 << ECHO_PIN)));
    while((PINB & (1 << ECHO_PIN))) {
        count++;
        _delay_us(1);
        if(count > 30000) return 0;
    }
    return count;
}

uint16_t measureDistance() {
    sendPulse();
    uint16_t duration = measureEcho();
    return (duration * 0.0343) / 2;
}