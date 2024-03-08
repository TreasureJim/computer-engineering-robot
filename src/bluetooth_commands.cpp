#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include "bluetooth.h"
#include "helpers.h"
#include "irsensor.h"

extern void (*finalise_command)();
extern uint8_t rx_buffer[];
extern uint8_t rx_num_bytes_goal;

void Set_Kp()
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

void Receive_Kp()
{
	rx_num_bytes_goal = 4;
	finalise_command = &Set_Kp;
}

void Set_IRCalibration()
{
	uint8_t min = *((uint8_t *)rx_buffer);
	uint8_t max = *(((uint8_t *)rx_buffer) + 1);
	EndienSwap(&min, sizeof(min));
	EndienSwap(&max, sizeof(max));

	IR_min = min;
	IR_max = max;

	char msg[30];
	uint8_t size = sprintf(msg, "[min %u max %u]\n", min, max);
	finalise_command = NULL;

	cli();
	Bluetooth_Send(msg, size + 1);
	sei();
}

void Receive_IRCalibration()
{
	rx_num_bytes_goal = 2;
	finalise_command = &Set_IRCalibration;
}