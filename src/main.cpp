#include <avr/delay.h>
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
	initialise_bluetooth();

	initialize_sensors();
	uint8_t min;
	uint8_t max;
	calibrateSensors(&min, &max);
	while (1)
	{
		for (float speed = 0.0f; speed < 1.0f; speed += 0.05f)
		{
			drive_motors(speed, 0.0f);
		}
	};
}
