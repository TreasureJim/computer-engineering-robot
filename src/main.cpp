#include <util/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "irsensor.h"
#include "motor.h"
#include <pins.h>
#include "bluetooth.h"

int main()
{
	sei();

	initialise_motors();
	Bluetooth_Initialise();

	initialize_sensors();
	uint8_t min;
	uint8_t max;
	calibrateSensors(&min, &max);

	while (1)
		;
}
