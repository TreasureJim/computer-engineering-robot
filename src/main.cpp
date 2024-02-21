#include "irsensor.h"

int main() {
	initialize_sensors();
	Serial.begin(9600);
	uint8_t minMid;
	uint8_t maxMid;
	uint8_t minLeft;
	uint8_t maxLeft;
	uint8_t minRight;
	uint8_t maxRight;
	getLimits(&minMid, &maxMid);

	while(1) {
		uint8_t avg = getAverageValue();
		uint8_t leftValue = analogread(leftSensor);
		uint8_t rightValue = analogread(rightSensor);
		float scaledValMiddle = getScaledValue(avg, &minMid, &maxMid);
		float scaledValLeft = getScaledValue(leftValue, &minLeft, &maxLeft);
		float scaledValRight = getScaledValue(rightValue, &minRight, &maxRight);
		Serial.print(avg);
		Serial.print(" ");
		Serial.print(scaledValMiddle);
		Serial.print(" ");
		Serial.print(scaledValLeft);
		Serial.print(" ");
		Serial.print(scaledValRight);
		Serial.print("\n");
	}
}