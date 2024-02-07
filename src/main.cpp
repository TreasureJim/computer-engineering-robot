#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "motor.h"
#include <pins.h>

volatile uint8_t dir = 255 / 2;

int main()
{
	_delay_ms(100);
	sei();

	initialise_motors();

	DDRD &= ~(0b1 << 2);
	PORTD |= 0b1 << 2;
	EICRA = 0b11;
	EIMSK = 0b1;

	drive_motors(0, dir);

	while (1)
		;
}

ISR(INT0_vect)
{
	cli();
	_delay_ms(100);
	dir += 10;
	drive_motors(100, dir);
	sei();
}