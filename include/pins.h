#ifndef PINS_H_
#define PINS_H_

#define __AVR_ATmega328P__
#include <avr/io.h>

///		MOTORS

// pin direction of B motor
#define MOTOR_DDR DDRD
// direction control pin of B motor
#define MOTOR_L_CTRL 4
// direction control pin of B motor
#define MOTOR_L_PWM 5
// direction control pin of A motor
#define MOTOR_R_CTRL 2
// PWM control pin of A motor
#define MOTOR_R_PWM 6

/// LINE SENSOR

/// BLUETOOTH

/// ULTRASONIC / DISTANCE SENSOR

#endif