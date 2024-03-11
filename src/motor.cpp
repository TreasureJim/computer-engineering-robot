#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "pins.h"

#define TIMER_TOP 0xFF
#define MOTOR_MIN_PWM 0

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
	uint8_t turn_factor_right = (direction + 1.0f) * ((255 - MOTOR_MIN_PWM) / 2.0f);
	uint8_t turn_factor_left = (255 - MOTOR_MIN_PWM) - turn_factor_right;

	float speed_adjuster = (1.0f - fabs(direction));

	// decrease overall speed when turning
	speed *= (1.0f - fabs(direction) * 0.05);

	if (direction < 0.0)
	{
		// left motor
		OCR0A = (MOTOR_MIN_PWM + turn_factor_left) * speed;
		// right motor
		OCR0B = (MOTOR_MIN_PWM + turn_factor_right) * speed * speed_adjuster;
	}
	else
	{
		// left motor
		OCR0A = (MOTOR_MIN_PWM + turn_factor_left) * speed * speed_adjuster;
		// right motor
		OCR0B = (MOTOR_MIN_PWM + turn_factor_right) * speed;
	}
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

void motorCalibration()
{
	// right motor
	OCR0A = 255;
	// left motor
	OCR0B = 0;
}