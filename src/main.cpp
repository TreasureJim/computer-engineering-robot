#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "irsensor.h"
#include "motor.h"
#include "pins.h"
#include "pid.h"

PIDController pidcontroller;

uint8_t Kp 10;
uint8_t Ki 0;
uint8_t Kd 0;

uint8_t min;
uint8_t max;
#include <pins.h>
#include "bluetooth.h"

int main()
{
	sei();

	initialise_motors();
	initialise_bluetooth();
	initialize_sensors();
	PIDController_Init(&pidcontroller, Kp, Ki, Kd);
	calibrateSensors(&min, &max);

	while (1)
		;
}
