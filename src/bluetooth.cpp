#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 9600
#include <util/setbaud.h>

void initialise_bluetooth()
{
	// Baud Rate
	UBRR0L = UBRRL_VALUE;
	UBRR0H = UBRRH_VALUE;

	UCSR0B = 0b1 << TXEN0 | 0b1 << TXCIE0 | 0b1 << RXCIE0 | 0b1 << RXEN0;

	UCSR0C = 0b11 << UCSZ00;

	DDRB = 0xff;
}

ISR(USART_RX_vect)
{
	char in = UDR0;

	if (in == '1')
	{
		PORTB = 0xff;

		while (!(UCSR0A & (1 << UDRE0)))
			;
		UDR0 = 'y';
	}
	else if (in == '0')
	{
		PORTB = 0x00;
		while (!(UCSR0A & (1 << UDRE0)))
			;
		UDR0 = 'n';
	}
}
