#include "ultrasonicsensor.h"
#include <avr/io.h>

#define SPEED_OF_SOUND 343

volatile uint8_t elapsedtimeMS;
/// @brief Initialization for the Ultrasonic Sensor
void Initialize_UltrasonicSensor() {
    // Set ECHO to input and TRIGGER to output
    DDRB |= (0 << ECHO_PIN) | (1 << TRIGGER_PIN);
}

/// @brief Sends a 10 microsecond pulse on the trigger pin
void sendPulse() {
    PORTB &= ~(1 << TRIGGER_PIN);
    _delay_us(2);
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIGGER_PIN);
}

/// @brief Measures time taken for the echo pin to detect
/// the sound wave sent by the trigger pin.
/// @return time taken for the echo pin to trigger
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

/// @brief Measures the distance the sound wave traveled
/// @return Distance in cm
uint16_t measureDistance() {
    sendPulse();
    uint16_t duration = measureEcho();
    return (duration * 0.0343) / 2;
}