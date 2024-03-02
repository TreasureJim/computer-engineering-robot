#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

void initialise_motors();

void drive_motors(float direction);

/// @brief Starts output on motor pins
void start_motors();

void cut_motors();

void motorCalibration();

#endif
