#include "irsensor.h"

int main() {
	initialize_sensors();
	Serial.begin(9600);
	uint8_t min;
	uint8_t max;
	getLimits(right_sensor, &min, &max);

	while(1) {
		uint8_t m_val = analogread(mid_sensor);
		float scaledVal = getScaledValue(m_val, &min, &max);
		Serial.print(m_val);
		Serial.print(" ");
		Serial.print(scaledVal);
		Serial.print("\n");
	}
}