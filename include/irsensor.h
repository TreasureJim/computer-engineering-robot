#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <Arduino.h>

void initialize_sensors();
uint8_t analogread(uint8_t channel);
uint8_t getAverageValue();
void getLimits(uint8_t *min, uint8_t *max);
void calibrateSensors(uint8_t *min, uint8_t *max);
float getScaledValue(uint8_t *min, uint8_t *max);

#endif