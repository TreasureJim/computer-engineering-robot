#include <avr/delay.h>
#include <avr/interrupt.h>

#include <Arduino.h>

#include "motor.h"
#include "pins.h"
#include "bluetooth.h"

#include <SoftwareSerial.h>

SoftwareSerial bluetooth(6, 7);
#define BLUETOOTH_BAUD 38400

int main()
{
	sei();

	// initialise_motors();
	// start_motors();

	// initialise_bluetooth();

	// DDRB = 0xff;
	// PORTB = 0xff;
	// _delay_ms(100);

	Serial.begin(9600);
	Serial.print("Sketch:   ");
	Serial.println(__FILE__);
	Serial.print("Uploaded: ");
	Serial.println(__DATE__);
	Serial.println(" ");

	bluetooth.begin(BLUETOOTH_BAUD);
	Serial.print("BTserial started at ");
	Serial.println(BLUETOOTH_BAUD);
	Serial.println(" ");

	char c;
	boolean NL = true;

	while (1)
	{
		// Read from the Bluetooth module and send to the Arduino Serial Monitor
		if (bluetooth.available())
		{
			c = bluetooth.read();
			Serial.write(c);
		}

		// Read from the Serial Monitor and send to the Bluetooth module
		if (Serial.available())
		{
			c = Serial.read();
			bluetooth.write(c);
		}
	}
}