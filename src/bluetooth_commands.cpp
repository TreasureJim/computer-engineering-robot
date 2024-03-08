#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include "bluetooth.h"
#include "helpers.h"

extern void (*finalise_command)();
extern uint8_t rx_buffer[];
extern uint8_t rx_num_bytes_goal;

void set_Kp()
{
	float received = *(float *)rx_buffer;
	EndienSwap(&received, sizeof(received));

	char msg[30];
	uint8_t size = sprintf(msg, "[received %.4f]\n", (double)received);

	cli();
	Bluetooth_Send(msg, size + 1);
	finalise_command = NULL;
	sei();

	// TODO: Set Kp
}

void receive_Kp()
{
	rx_num_bytes_goal = 4;
	finalise_command = &set_Kp;
}