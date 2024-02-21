#include "irsensor.h"
int main() {
	initialize_sensors();
	uint8_t *sensorValues = (uint8_t*)malloc(NUM_OF_SENSORS * sizeof(uint8_t));
	while(1) {
		readIRsensors(sensorValues);
	}
	return 0;
}