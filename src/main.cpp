#include "irsensor.h"
#include "motor.h"
int main() {
	initialise_motors();	
	initialize_sensors();
	uint8_t min;
	uint8_t max;
	calibrateSensors(&min, &max);
	while(1) {
	}
}