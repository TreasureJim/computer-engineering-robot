#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <Arduino.h>
//PORT C, pin 0
#define right_sensor PINC0
//PORT C, pin 1
#define left_sensor PINC1
//PORT C, pin 2
#define mid_sensor PINC2

#define NUM_OF_SENSORS 3

void initialize_sensors();
uint8_t analogread(uint8_t channel);
uint8_t getAverageValue();
void getLimits(uint8_t channel, uint8_t *min, uint8_t *max);
float getScaledValue(uint8_t val, uint8_t *min, uint8_t *max);
#endif