#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "pins.h"

#define MOTOR_L_BASE_SPEED 100
#define MOTOR_R_BASE_SPEED 100

#define TIMER_TOP 0xFF

void initialise_motors()
{
	// set motor pins to output
	MOTOR_DDR |= 1 << MOTOR_L_CTRL | 1 << MOTOR_L_PWM | 1 << MOTOR_R_CTRL | 1 << MOTOR_R_PWM;
	MOTOR_PIN_OUTPUT &= ~(1 << MOTOR_L_CTRL | 1 << MOTOR_R_CTRL);

	// Motors use timer 0 to run

	// COM0A0 - Clear OC0A on compare match when up-counting. Set OC0A on compare match when down-counting.
	// COM0B0 - Set OC0B on compare match when up-counting. Clear OC0B on compare match when down-counting.
	// WGM00 - phase correct PWM mode
	TCCR0A = 0b10 << COM0A0 | 0b11 << COM0B0 | 0b01 << WGM00;

	// WGM02 - phase correct PWM mode
	// CS00 - 256 prescaler
	TCCR0B = 0b0 << WGM02 | 0b100 << CS00;
}

void drive_motors(uint8_t speed, uint8_t direction)
{
	uint8_t turn_factor = TIMER_TOP * (direction / 255.0);

	// left motor
	OCR0A = turn_factor;
	// right motor
	OCR0B = OCR0A;
}

// Stops output on motor pins
// Use initialise_motors to restart
void cut_motors()
{
	TCCR0A &= ~(0b11 << COM0A0 | 0b11 << COM0B0);
}