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

PIDController pidcontroller;
float Kp = 7.0f, Ki = 0.0f, Kd = 3.25f;
float Hz = 10.0f;

int main()
{
	// Initializations
	sei();
	DDRB |= (1 << PINB2);
	Bluetooth_Initialise();
	IR_InitialiseSensor();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd, Hz);
	// Initialize_UltrasonicSensor();
	PID_Timer_Init();
	initialise_motors();

	// Sensor Calibration routine
	IR_CalibrateSensors(&IR_min, &IR_max);
	// IR_min = 60;
	// IR_max = 255;
	SetError();
	_delay_ms(5000);
	ClearError();


	// char msg[20];
	// uint8_t msg_size = sprintf(msg, "min %u max %u\n", IR_min, IR_max);
	// Bluetooth_Send(msg, msg_size + 1);
	// _delay_ms(1000);

	// Initial motor values
	start_motors();
	drive_motors(0.7, 0.0);
	PID_Start();
	while(1) {

	};
}

uint8_t lap_counter = 0;

RunningDiagnostics diagnostics;
ISR(TIMER1_COMPA_vect)
{
	diagnostics.IR = IR_GetScaledValue(&IR_min, &IR_max);
	// if (diagnostics.IR > 0.8f)
	// {
	// 	_delay_ms(1);
	// 	diagnostics.IR = IR_GetScaledValue(&IR_min, &IR_max);
	// 	if (diagnostics.IR < 0.4f)
	// 	{
	// 		lap_counter++;
	// 		_delay_ms(1);
	// 		if (lap_counter > GOAL_LAPS)
	// 		{
	// 			cli();
	// 			cut_motors();
	// 			PID_Stop();
	// 			BlinkLED();
	// 			exit(1);
	// 		}	
	// 	}
	// }

	// if (measureDistance() < 20)
	// {
	// 	cut_motors();
	// 	return;
	// }
	diagnostics.PID = PIDController_Compute(&pidcontroller, 0.65f, diagnostics.IR);

	start_motors();
	drive_motors(1.0f, diagnostics.PID);

	char msg[30];
	uint8_t msg_size = sprintf(msg, "IR: %f PID: %f\n", (double)diagnostics.IR, (double)diagnostics.PID);
	Bluetooth_Send(msg, msg_size + 1);
}