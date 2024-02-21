#include "irsensor.h"

int main() {
	initialize_sensors();
	Serial.begin(9600);
	uint8_t min;
	uint8_t max;
	getLimits(&min, &max);

	while(1) {
		uint8_t avg = getAverageValue();
		float scaledVal = getScaledValue(avg, &min, &max);
		Serial.print(avg);
		Serial.print(" ");
		Serial.print(scaledVal);
		Serial.print("\n");
	}
}