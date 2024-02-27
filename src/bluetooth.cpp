#include <avr/io.h>

void initialise_bluetooth()
{
	// Baud Rate
	// UBRR0 = 103;
	UBRR0 = 25;

	UCSR0B = 0b1 << TXEN0 | 0b1 << RXCIE0 | 0b1 << RXEN0;
}