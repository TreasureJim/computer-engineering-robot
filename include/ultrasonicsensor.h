#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pins.h"
#include <stdint.h>

void Initialize_UltrasonicSensor();
uint16_t measureEcho();
uint16_t measureDistance();
void sendPulse();

#endif