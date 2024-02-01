#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

void initialise_motors();

void drive_motors(uint8_t speed, uint8_t direction);

#endif
