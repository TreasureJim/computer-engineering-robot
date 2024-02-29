#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUD 9600
#include <util/setbaud.h>

void initialise_bluetooth()
{
	// Baud Rate
	UBRR0L = UBRRL_VALUE;
	UBRR0H = UBRRH_VALUE;

	UCSR0B = 0b1 << TXEN0 | 0b1 << TXCIE0 | 0b1 << RXCIE0 | 0b1 << RXEN0;

	UCSR0C = 0b11 << UCSZ00;
}

void send_bytes(uint8_t *bytes, uint8_t n)
{
	// TODO
}

void (*finalise_command)() = nullptr;
uint8_t received_bytes[4];
uint8_t byte_count = 0;
uint8_t byte_count_goal = 0;

ISR(USART_RX_vect)
{
	if (!finalise_command)
	{
		return receive_command();
	}

	// we are processing a command
	if (byte_count <= byte_count_goal)
	{
		// receive a byte
		received_bytes[byte_count] = UDR0;
		byte_count++;
		return;
	}
	else
	{
		(*finalise_command)();
	}
}

void receive_command()
{
	byte_count = 0;
	byte_count_goal = 0;

	char command = UDR0;

	switch (command)
	{
	case 0x02:
		recieve_Kp();
		break;
	}
}

void recieve_Kp()
{
	byte_count_goal = 3;
	finalise_command = &set_Kp;
}

void set_Kp()
{
	float received;
	memcpy(&received, received_bytes, 4);
	// TODO: Set Kp
}