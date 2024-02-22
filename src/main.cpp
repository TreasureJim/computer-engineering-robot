#include "irsensor.h"

int main() {
	initialize_sensors();
	Serial.begin(9600);
	uint8_t minRightMid;
	uint8_t maxRightMid;
	uint8_t minLeftMid;
	uint8_t maxLeftMid;
	uint8_t minMidMid;
	uint8_t maxMidMid;
	uint8_t minLeft;
	uint8_t maxLeft;
	uint8_t minRight;
	uint8_t maxRight;
	getLimits(rightMidSensor, &minRightMid, &maxRightMid);
	getLimits(leftMidSensor, &minLeftMid, &maxLeftMid);
	getLimits(midMidSensor, &minMidMid, &maxMidMid);
	getLimits(leftSensor, &minLeft, &maxLeft);
	getLimits(rightSensor, &minRight, &maxRight);

	while(1) {
		uint8_t avg = getAverageValue();
		uint8_t leftValue = analogread(leftSensor);
		uint8_t rightValue = analogread(rightSensor);
		float scaledValMiddle = getScaledValue(avg, &minMidMid, &maxMidMid);
		float scaledValLeft = getScaledValue(leftValue, &minLeft, &maxLeft);
		float scaledValRight = getScaledValue(rightValue, &minRight, &maxRight);
		Serial.print("Left: ");
		// Serial.print(" ");
		Serial.print(scaledValLeft);
		Serial.print(" ");
		Serial.print("Mid: ");
		Serial.print(scaledValMiddle);
		Serial.print(" ");
		Serial.print("Right: ");
		// Serial.print(" ");
		Serial.print(scaledValRight);
		Serial.print("\n");
	}
}