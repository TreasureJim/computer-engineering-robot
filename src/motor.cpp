#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "pins.h"

#define MOTOR_L_BASE_SPEED 100
#define MOTOR_R_BASE_SPEED 100

#define TIMER_TOP 0x00FF

void initialise_motors()
{
	sei();

	// set motor pins to output
	MOTOR_DDR |= 1 << MOTOR_L_CTRL | 1 << MOTOR_L_PWM | 1 << MOTOR_R_CTRL | 1 << MOTOR_R_PWM;
	MOTOR_PIN_OUTPUT &= ~(1 << MOTOR_L_CTRL | 1 << MOTOR_R_CTRL);

	// Motors use timer 1 to run

	// COM1A0 / COM1B0 - Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when down counting
	// WGM11 - phase and frequency correct PWM mode
	TCCR1A = 0b10 << COM1A0 | 0b10 << COM1B0 | 0b01 << WGM11;

	// WGM12 - phase and frequency correct PWM mode
	// CS10 - 256 prescaler
	TCCR1B = 0b00 << WGM12 | 0b011 << CS10;

	// OCIE1A / B - Output Compare A / B Match Interrupt Enable
	// TIMSK1 = 0b1 << OCIE1B | 0b1 << OCIE1A;

	ICR1 = 0xffff;
}

void drive_motors(uint8_t speed, uint8_t direction)
{
	Serial.print("dir ");
	Serial.println(direction);

	// disable left motor
	if (direction == 255)
	{
		TIMSK1 = 0b0;
		MOTOR_PIN_OUTPUT = (MOTOR_PIN_OUTPUT & ~(0b1 << MOTOR_L_PWM)) | 0b1 << MOTOR_R_PWM;
		return;
	}
	// disable left motor
	else if (direction == 0)
	{
		TIMSK1 = 0b0;
		MOTOR_PIN_OUTPUT = (MOTOR_PIN_OUTPUT & ~(0b1 << MOTOR_R_PWM)) | 0b1 << MOTOR_L_PWM;
		return;
	}
	// enable both motors
	else
	{
		TIMSK1 = 0b1 << OCIE1B | 0b1 << OCIE1A;
	}

	double percent = direction / 255.0;
	// Serial.println(percent);

	// left motor
	OCR1A = (uint16_t)(TIMER_TOP * (1 - percent));
	Serial.println(OCR1A);

	// right motor
	OCR1B = (uint16_t)(TIMER_TOP * percent);
	// Serial.println(OCR1B);
}

void cut_motors()
{
	TIMSK1 = 0b0;
}

ISR(TIMER1_COMPA_vect)
{
	MOTOR_PIN_OUTPUT ^= 1 << MOTOR_L_PWM;
}

ISR(TIMER1_COMPB_vect)
{
	MOTOR_PIN_OUTPUT ^= 1 << MOTOR_R_PWM;
}
