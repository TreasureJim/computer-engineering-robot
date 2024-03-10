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
	DDRB |= (1 << PINB2);
	sei();
	initialise_motors();
	Initialize_UltrasonicSensor();
	start_motors();
	drive_motors(0.5f, 0.0f);
	while (1) {
		// Acceptance test for the sensor required.
		// This code will be used for the acceptance test.
		uint16_t distance = measureDistance();
		if(distance < 10) {
			cut_motors();
			SetError();
		}
		else {
			start_motors();
			ClearError();
		}
		_delay_ms(100);
	}

	return 0;
}

RunningDiagnostics diagnostics;
ISR(TIMER1_COMPA_vect)
{
	diagnostics.IR = IR_GetScaledValue(&IR_min, &IR_max);
	diagnostics.PID = PIDController_Compute(&pidcontroller, 0.5f, diagnostics.IR);

	// Bluetooth_Send(&diagnostics, sizeof(diagnostics));
}