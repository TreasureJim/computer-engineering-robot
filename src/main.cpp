#include "irsensor.h"
#include "motor.h"
int main() {
	initialise_motors();
	initialize_sensors();
	Serial.begin(9600);
	uint8_t min;
	uint8_t max;
	calibrateSensors(&min, &max);
	while(1) {
		float scaledAverage = getScaledValue(&min, &max);
		if(scaledAverage < 0.25) {
			Serial.print("White\n");
			_delay_ms(10);
		}
		else if(scaledAverage < 0.75 && scaledAverage > 0.25) {
			Serial.print("Grey\n");
			_delay_ms(10);
		}
		else if(scaledAverage > 0.75) {
			Serial.print("Black\n");
			_delay_ms(10);
		}
	}
}