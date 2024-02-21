#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "motor.h"
#include "pins.h"
#include "bluetooth.h"

int main()
{
	sei();

	// initialise_motors();
	// start_motors();

	initialise_bluetooth();

	DDRB = 0xff;
	PORTB = 0xff;
	_delay_ms(100);

	while (1)
	{
		while (!(UCSR0A & (1 << RXC0)))
			;

		PORTB ^= 0xff;
	}
}