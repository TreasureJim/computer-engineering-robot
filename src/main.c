#include <avr/delay.h>
#include <util/delay.h>

#include "motor.h"

int main() {
	initialise_motors();

	drive_motors(100, 0);

	while(1) {}

	// for (int angle = 0; angle <= 100; angle += 1) {
	// 	drive_motors(100, angle);
	// 	_delay_ms(100);
	// }
}
