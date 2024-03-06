#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "irsensor.h"
#include "motor.h"
#include "pins.h"
#include "pid.h"
#include "bluetooth.h"

PIDController pidcontroller;

float Kp = 0.1f, Ki = 0.00f, Kd = 0.0f;
float Hz = 10.0f;

uint8_t min;
uint8_t max;

void SetError();
void ClearError();
void PID_Start();
void PID_Stop();

int main()
{
	sei();

	// Error LED
	DDRB |= 0b1 << PORTB2;

	initialise_motors();
	initialise_bluetooth();

	initialize_sensors();
	uint8_t min;
	uint8_t max;
	calibrateSensors(&min, &max);
	while (1)
		;
}

void PID_Start()
{
	TIMSK1 |= 0b1 << OCIE1A;
}

void PID_Stop()
{
	TIMSK1 &= ~(0b1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	float output = PIDController_Compute(&pidcontroller, 0.5f, IR_GetScaledValue(min, max));
	Bluetooth_Send(&output, sizeof(output));
}

void SetError()
{
	PORTB |= 0b1 << PORTB2;
}

void ClearError()
{
	PORTB &= ~(0b1 << PORTB2);
}