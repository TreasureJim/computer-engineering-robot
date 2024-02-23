#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <Arduino.h>
//PORT C, pin 0
#define rightMidSensor PINC2
//PORT C, pin 1
#define leftMidSensor PINC1
//PORT C, pin 2
#define midMidSensor PINC4
//PORT C, pin 3
#define leftSensor PINC0
//PORTC, pin 4
#define rightSensor PINC3
#define NUM_OF_SENSORS 3

void initialize_sensors();
uint8_t analogread(uint8_t channel);
uint8_t getAverageValue();
void getLimits(uint8_t *min, uint8_t *max);
void calibrateSensors(uint8_t *min, uint8_t *max);
float getScaledValue(uint8_t *min, uint8_t *max);

#endif