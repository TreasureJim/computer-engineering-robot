#include <avr/interrupt.h>

#include "motor.h"
#include "pins.h"
#include "bluetooth.h"

int main()
{
	sei();

	initialise_bluetooth();

	while (1)
		;
}
