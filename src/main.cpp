#include <util/delay.h>
#include <avr/interrupt.h>
#include "irsensor.h"
#include "motor.h"
#include "pins.h"
#include "pid.h"
#include "bluetooth.h"
#include <stdio.h>

PIDController pidcontroller;

float Kp = 1.0f, Ki = 0.00f, Kd = 0.0f;
float Hz = 10.0f;

int main()
{
	sei();

	// Error LED
	DDRB |= 0b1 << PORTB2;

	initialise_motors();
	Bluetooth_Initialise();
	IR_InitialiseSensor();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd, Hz);

	// SetError();
	IR_GetLimits(&IR_min, &IR_max);
	// ClearError();

	char msg[] = "min xxx max xxx\n";
	uint8_t msg_size = sprintf(msg, "min %u max %u\n", IR_min, IR_max);
	Bluetooth_Send(msg, msg_size + 1);

	// start_motors();
	// drive_motors(0.5f, 0.0f);
	PID_Start();

	while (1)
		;

	return 0;
}

RunningDiagnostics diagnostics;
ISR(TIMER1_COMPA_vect)
{
	diagnostics.IR = IR_GetScaledValue(IR_min, IR_max);
	diagnostics.PID = PIDController_Compute(&pidcontroller, 0.5f, diagnostics.IR);

	// Bluetooth_Send(&diagnostics, sizeof(diagnostics));
}