#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>

///		MOTORS

// pin direction of B motor
#define MOTOR_DDR DDRD
#define MOTOR_PIN_OUTPUT PORTD

// direction control pin of B / left motor
#define MOTOR_L_CTRL 4
// direction control pin of B / left motor
// On OC0B
#define MOTOR_L_PWM 5

// direction control pin of A / right motor
#define MOTOR_R_CTRL 2
// PWM control pin of A / right motor
// On OC0A
#define MOTOR_R_PWM 6

/// LINE SENSOR

/// BLUETOOTH

/// ULTRASONIC / DISTANCE SENSOR

#endif
