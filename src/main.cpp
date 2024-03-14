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
#include "helpers.h"
#include <stdlib.h>

#define GOAL_LAPS 1
#define PID_INTERRUPT_HZ 30
RunningDiagnostics diagnostics;
PIDController pidcontroller;
float Kp = 7.5f, Ki = 0.0f, Kd = 5.0f;
float Hz = 10.0f;
uint8_t counter = 0;
int main()
{
	// Initializations
	sei();

	DDRB |= (1 << PINB2);
	ClearError();

	Bluetooth_Initialise();
	IR_InitialiseSensor();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd, Hz);
	// Initialize_UltrasonicSensor();
	initialise_motors();

	IR_min = 0x22;
	IR_max = 0xf0;

	_delay_ms(4000);

	// Sensor Calibration routine
	// IR_CalibrateSensors(&IR_min, &IR_max);

	char msg[] = "Running\n";
	Bluetooth_Send(msg, sizeof(msg));
	_delay_ms(100);

	PID_Timer_Init();
	PID_Start();
	while (1)
		;
}

ISR(TIMER1_COMPA_vect)
{
	// counter++;
	// if ((counter % 8) == 0)
	// {
	// 	if (measureDistance() < 20)
	// 	{
	// 		drive_motors(0.0, 0.0);
	// 		return;
	// 	}
	// }

	/*
	If ultrasonic is < 20 stop, rotate on spot, set new goal value, start driving
	*/

	diagnostics.IR = IR_GetScaledValue(&IR_min, &IR_max);
	diagnostics.PID = PIDController_Compute(&pidcontroller, 0.62f, diagnostics.IR);
	drive_motors(1.0f, diagnostics.PID);
}