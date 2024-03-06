#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "pins.h"

#define MOTOR_L_BASE_SPEED 100
#define MOTOR_R_BASE_SPEED 100

#define TIMER_TOP 0xFF
#define MOTOR_MIN_PWM 150

/// @brief Initialise motor and timer registers
void initialise_motors()
{
	// set motor pins to output
	MOTOR_DDR |= 1 << MOTOR_L_CTRL | 1 << MOTOR_L_PWM | 1 << MOTOR_R_CTRL | 1 << MOTOR_R_PWM;
	MOTOR_PIN_OUTPUT &= ~(1 << MOTOR_L_CTRL | 1 << MOTOR_R_CTRL);

	// Motors use timer 0 to run

	// COM0A0 - Clear OC0A on compare match when up-counting. Set OC0A on compare match when down-counting.
	// COM0B0 - Set OC0B on compare match when up-counting. Clear OC0B on compare match when down-counting.
	// WGM00 - phase correct PWM mode
	TCCR0A = 0b01 << WGM00;

	// WGM02 - phase correct PWM mode
	// CS00 - 256 prescaler
	TCCR0B = 0b0 << WGM02 | 0b010 << CS00;
}

/// @brief Change the direction of the car by changing the PWM to the motors
/// @param speed float between 0 and 1 which changes the overall speed
/// @param direction float between -1 and 1 where -1 is completely left and 1 is completely right and 0 is straight
void drive_motors(float speed, float direction)
{
	// float speed_rel = (float)speed / 255.0f;
	uint8_t turn_factor_right = (direction + 1.0f) * ((256 - MOTOR_MIN_PWM) / 2.0f);
	uint8_t turn_factor_left = (256 - MOTOR_MIN_PWM) - turn_factor_right;

	// left motor
	OCR0A = (MOTOR_MIN_PWM + turn_factor_right) * speed;
	// OCR0A = 100;
	// OCR0B = OCR0A;
	// right motor
	OCR0B = (MOTOR_MIN_PWM + turn_factor_left) * speed;
}

/// @brief Starts output on motor pins
void start_motors()
{
	TCCR0A |= 0b10 << COM0A0 | 0b10 << COM0B0;
}

/// @brief Stops output on motor pins
void cut_motors()
{
	TCCR0A &= ~(0b11 << COM0A0 | 0b11 << COM0B0);
}

void motorCalibration() {
	//left motor
	OCR0A = 255; 
	//right motor
	OCR0B = 0;
}