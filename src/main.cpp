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
float Kp = 10.0f, Ki = 0.00f, Kd = 0.0f;
float Hz = 10.0f;

int main()
{
	sei();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd, Hz);
	initialise_motors();
	IR_InitialiseSensor();
	Initialize_UltrasonicSensor();
	Bluetooth_Initialise();
	IR_CalibrateSensors(&IR_min, &IR_max);
	PID_Timer_Init();
	PID_Start();
	while(1) {

	};
	return 0;
}

RunningDiagnostics diagnostics;
ISR(TIMER1_COMPA_vect)
{
	diagnostics.IR = IR_GetScaledValue(&IR_min, &IR_max);
	diagnostics.PID = PIDController_Compute(&pidcontroller, 0.6f, diagnostics.IR);

	drive_motors(0.3f, diagnostics.PID);

	char msg[30];
	uint8_t msg_size = sprintf(msg, "IR: %.2f \r\n PID: %.2f\n", (double)diagnostics.IR, (double)diagnostics.PID);
	Bluetooth_Send(msg, 30);
}