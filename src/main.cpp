#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "motor.h"
#include <pins.h>

int main()
{
	sei();

	initialise_motors();
	// start_motors();
}