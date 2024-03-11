#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include "bluetooth.h"
#define BAUD 9600
#include <util/setbaud.h>
#include "helpers.h"

#define RX_BUFFER_SIZE 10
#define TX_BUFFER_SIZE 40

void Bluetooth_Initialise()
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

void Bluetooth_Send(void *data, uint8_t n)
{
	// disable receiving interrupt
	UCSR0B &= ~(0b1 << RXEN0);
	memcpy(tx_buffer, data, n);

	tx_byte_count_goal = n - 1;
	tx_byte_count = 1;

	while (!(UCSR0A & (1 << UDRE0)))
		; /* Wait for empty transmit buffer */

	// start first transmission
	UDR0 = tx_buffer[0];
}

void Bluetooth_SendIntValues(void *data, uint8_t n)
{
	unsigned int number = *(uint8_t *)data;
	char string[4];
	sprintf(string, "%u", number);
	Bluetooth_Send(&string, sizeof(string));
}

void Bluetooth_SendFloatValues(void *data, uint8_t n)
{
	float number = *(float *)data;
	char string[4];
	sprintf(string, "%.2f", (double)number);
	Bluetooth_Send(&string, sizeof(string));
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

	sei();
}

#include "bluetooth_commands.h"

void (*finalise_command)() = NULL;
uint8_t rx_buffer[RX_BUFFER_SIZE];
// manages the index counter for rx_bytes
uint8_t rx_byte_count = 0;
uint8_t rx_num_bytes_goal = 0;

void receive_command()
{
	rx_byte_count = 0;
	rx_num_bytes_goal = 0;

	// uint8_t command = UDR0;

	// switch (command)
	// {
	// case 0x02:
	// 	Receive_Kp();
	// 	break;
	// 	// case 0x03:
	// 	// 	break;
	// 	// case 0x04:
	// 	// 	break;
	// case 0x06:
	// 	Receive_IRCalibration();
	// 	break;
	// }
}

ISR(USART_RX_vect)
{
	if (!finalise_command)
	{
		return receive_command();
	}

	// we are processing a command
	if (rx_byte_count < rx_num_bytes_goal)
	{
		PORTB ^= 0b1 << PORTB2;
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
