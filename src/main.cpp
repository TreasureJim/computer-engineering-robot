#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "motor.h"

volatile uint8_t dir = 0;

#include <pins.h>

int main()
{
	Serial.begin(9600);
	initialise_motors();

	PORTD |= 1 << 2;
	DDRD &= ~(1 << 2);
	EICRA = 0b11;
	EIMSK = 0b1;

	_delay_ms(100);
	dir = 255;
	drive_motors(100, dir);

	MOTOR_PIN_OUTPUT = 0xff;

	while (1)
	{
		// drive_motors(100, dir);
		// _delay_ms(2000);
		// for (int dir = 0; dir <= 100; dir++)
		// {
		// }
	}
}

ISR(INT0_vect)
{
	cli();
	_delay_ms(100);
	dir += 10;
	// Serial.println(dir);
	drive_motors(0, dir);
	sei();
}