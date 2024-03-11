#include <util/delay.h>
#include <avr/interrupt.h>
#include "irsensor.h"
#include "motor.h"
#include "pins.h"
#include "pid.h"
#include "bluetooth.h"
#include "ultrasonicsensor.h"
#include "helpers.h"
#include <stdio.h>
#include <stdint.h>

PIDController pidcontroller;
float Kp = 1.0f, Ki = 0.00f, Kd = 0.0f;
float Hz = 10.0f;

int main()
{
	sei();

	DDRB |= (1 << PINB2);

	Bluetooth_Initialise();
	IR_InitialiseSensor();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd, Hz);
	Initialize_UltrasonicSensor();
	PID_Timer_Init();
	// initialise_motors();

	SetError();
	IR_CalibrateSensors(&IR_min, &IR_max);
	ClearError();

	char msg[20];
	uint8_t msg_size = sprintf(msg, "min %u max %u\n", IR_min, IR_max);
	Bluetooth_Send(msg, msg_size + 1);
	_delay_ms(1000);

	// start_motors();
	// drive_motors(0.7, 0.0);

	PID_Start();

	while (1)
		;
}

RunningDiagnostics diagnostics;
ISR(TIMER1_COMPA_vect)
{
	diagnostics.IR = IR_GetScaledValue(&IR_min, &IR_max);
	// diagnostics.PID = PIDController_Compute(&pidcontroller, 0.5f, diagnostics.IR);
	diagnostics.PID = 0.738f;

	// drive_motors(0.7f, diagnostics.PID);

	char msg[30];
	uint8_t msg_size = sprintf(msg, "IR: %.2f PID: -0.738\n", (double)diagnostics.IR);
	Bluetooth_Send(msg, msg_size + 1);
}