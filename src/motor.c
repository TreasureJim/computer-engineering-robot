#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "pins.h"

#define MOTOR_L_BASE_SPEED 100
#define MOTOR_R_BASE_SPEED 100

void initialise_motors() {
	sei();

	// set motor pins to output
	MOTOR_DDR |= 1 << MOTOR_L_CTRL | 1 << MOTOR_L_PWM | 1 << MOTOR_R_CTRL | 1 << MOTOR_R_PWM;
	MOTOR_PIN_OUTPUT |= 1 << MOTOR_L_CTRL | 1 << MOTOR_R_CTRL;

	// Motors use timer 1 to run
	// 
	// It is recommended to use the phase and frequency correct mode instead of the phase correct mode when changing the TOP value while the Timer/Counter is running. When using a static TOP value there are practically no differences between the two modes of operation

	// COM1A0 / COM1B0 - Clear OC1A/OC1B on compare match when up-counting. Set OC1A/OC1B on compare match when down counting
	// WGM11 - phase and frequency correct PWM mode
	TCCR1A = 0b10 << COM1A0 | 0b10 << COM1B0 | 0b00 << WGM11;
	
	// WGM12 - phase and frequency correct PWM mode
	// CS10 - 256 prescaler
	TCCR1B = 0b10 << WGM12 | 0b011 << CS10;

	// OCIE1A / B - Output Compare A / B Match Interrupt Enable
	TIMSK1 = 0b1 << OCIE1B | 0b1 << OCIE1A;

	ICR1 = 0xffff;
}

void drive_motors(uint8_t speed, uint8_t direction) {
	// left motor
	OCR1A = (uint16_t) (ICR1 / (float) (100 - direction));
	// right motor
	OCR1B = (uint16_t) (ICR1 / (float) direction);
}

void cut_motors() {
	OCR1A, OCR1B = 0;
}

ISR(TIMER1_COMPA_vect) {
	// MOTOR_PIN_OUTPUT ^= MOTOR_L_PWM;
	MOTOR_PIN_OUTPUT ^= 1 << MOTOR_L_PWM;
}

ISR(TIMER1_COMPB_vect) {
	MOTOR_PIN_OUTPUT ^= 1 << MOTOR_R_PWM;
}
