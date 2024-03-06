#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define BAUD 9600
#include <util/setbaud.h>

#define TX_BUFFER_SIZE 4

void initialise_bluetooth()
{
	// Baud Rate
	UBRR0L = UBRRL_VALUE;
	UBRR0H = UBRRH_VALUE;

	UCSR0B = 0b1 << TXEN0 | 0b1 << TXCIE0 | 0b1 << RXCIE0 | 0b1 << RXEN0;

	UCSR0C = 0b11 << UCSZ00;
}

uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_byte_count = 0;
uint8_t tx_byte_count_goal = 0;

void send_bytes(uint8_t *data, uint8_t n)
{
	// disable receiving interrupt
	// UCSR0B &= ~(0b1 << RXCIE0);

	memcpy(tx_buffer, data, n);

	tx_byte_count_goal = n - 1;
	tx_byte_count = 1;

	// wait for empty transmission buffer
	while (!(UCSR0A & (1 << RXC0)))
		;
	// start first transmission
	UDR0 = tx_buffer[0];
}

ISR(USART_TX_vect)
{
	cli();

	// if sent enough bytes exit
	if (tx_byte_count >= tx_byte_count_goal)
	{
		UCSR0B |= 0b1 << RXEN0;
		return;
	}

	UDR0 = tx_buffer[tx_byte_count];
	tx_byte_count++;
}

void (*finalise_command)() = nullptr;
uint8_t rx_buffer[4];
// manages the index counter for rx_bytes
uint8_t rx_byte_count = 0;
uint8_t rx_byte_count_goal = 0;

void set_Kp()
{
	float received;
	memcpy(&received, rx_buffer, 4);
	// TODO: Set Kp
}

void recieve_Kp()
{
	rx_byte_count_goal = 3;
	finalise_command = &set_Kp;
}

void receive_command()
{
	rx_byte_count = 0;
	rx_byte_count_goal = 0;

<<<<<<< HEAD
	char command = UDR0;

	switch (command)
	{
	case 0x02:
		recieve_Kp();
		break;
	}
=======
	uint8_t command = UDR0;

	// switch (command)
	// {
	// case 0xD8:
	// 	recieve_Kp();
	// 	break;
	// }
>>>>>>> bluetooth-fix
}

ISR(USART_RX_vect)
{
	if (!finalise_command)
	{
		return receive_command();
	}

	// we are processing a command
	if (rx_byte_count <= rx_byte_count_goal)
	{
		// receive a byte
		rx_buffer[rx_byte_count] = UDR0;
		rx_byte_count++;
		return;
	}
	else
	{
		(*finalise_command)();
	}
}