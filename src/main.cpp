#include <Arduino.h>
#include <stdio.h>

#define left_sensor 14
#define right_sensor 15
#define mid_sensor 16

void setup()
{
	pinMode(left_sensor, INPUT);
	pinMode(right_sensor, INPUT);
	pinMode(mid_sensor, INPUT);
	Serial.begin(9600);
}

void loop()
{
	int sensors[3] = {left_sensor, right_sensor, mid_sensor};
	int sensors_values[3];
	for(int i = 0; i < 10000; i++) {
		for(int i = 0; i < 3; i++) {
			sensors_values[i] = analogRead(sensors[i]);
			Serial.print("Sensor value at ");
			Serial.print(sensors[i]);
			Serial.print(": ");
			Serial.print(sensors_values[i]);
			Serial.print(" ");
			
		}
	}
	Serial.print("\n");
	delay(200);

	// uint16_t sensor_value = analogRead(right_sensor);
	// Serial.print("Sensor value: ");
	// Serial.print(sensor_value);
	// Serial.print("\n");
	// delay(200);
}