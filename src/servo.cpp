#include <avr/io.h>
#include "helpers.h"
#include <util/delay.h>
#include <Arduino.h>

void Servo_Initialise()
{
	DDRB |= 0b1 << PORTB2;
}

void Servo_Rotate(uint16_t angle)
{
	uint16_t pulsewidth = angle * 11 + 500; // calculate the value of pulse width
	PORTB |= 0b1 << PORTB2;
	delayMicroseconds(pulsewidth);
	PORTB &= ~(0b1 << PORTB2);
	// _delay_ms(20);
}