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

	initialise_motors();
	initialise_bluetooth();
	IR_IntialiseSensor();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd, Hz);

	// begin calibration
	IR_CalibrateSensors(&min, &max);

	// init PID timer
	TCCR1B = 0b11 << WGM12 | 0b011 << CS10;
	ICR1 = 25000;

	// start_motors();
	// drive_motors(0.5f, 0.0f);

	PID_Start();

	while (1)
		;
}

void PID_Start()
{
	TIMSK1 = 0b1 << ICIE1;
}

void PID_Stop()
{
	TIMSK1 &= ~(0b1 << ICIE1);
}

ISR(TIMER1_COMPA_vect)
{
	float output = PIDController_Compute(&pidcontroller, 0.5f, IR_GetScaledValue(min, max));
	Bluetooth_SendBytes((uint8_t *)&output, sizeof(output));
}