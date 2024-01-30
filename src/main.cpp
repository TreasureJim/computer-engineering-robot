#include <Arduino.h>

#define SERVO_PIN 10
int pulsewidth;

void procedure(int angle);

void setup()
{
	pinMode(SERVO_PIN, OUTPUT);
	pinMode(9, OUTPUT);
}

void loop()
{
	procedure(90);
	delay(100);
}

void procedure(int angle)
{
	pulsewidth = angle * 11 + 500;
	digitalWrite(SERVO_PIN, HIGH);
	delayMicroseconds(pulsewidth);
	digitalWrite(SERVO_PIN, LOW);
	delay(20 - pulsewidth / 1000);
}