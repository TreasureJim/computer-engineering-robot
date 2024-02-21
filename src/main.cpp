#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "motor.h"
#include "pins.h"
#include "pid.h"

PIDController pidcontroller;

#define Kp 10
#define Ki 0
#define Kd 0

int main()
{
	sei();

	initialise_motors();
	// start_motors();

	PIDController_Init(&pidcontroller, Kp, Ki, Kd);
}