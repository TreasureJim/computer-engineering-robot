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

int main()
{
	sei();

	initialise_motors();
	initialize_sensors();

	PIDController_Init(&pidcontroller, Kp, Ki, Kd);
	calibrateSensors(&min, &max);

	// start_motors();
	while (1)
	{
	}
}