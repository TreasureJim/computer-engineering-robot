#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <Arduino.h>

void IR_IntialiseSensor();
void IR_CalibrateSensors(uint8_t *min, uint8_t *max);
float IR_GetScaledValue(uint8_t min, uint8_t max);

uint8_t IR_ReadSensor(uint8_t channel);
uint8_t IR_GetAverageValue();
void IR_GetLimits(uint8_t *min, uint8_t *max);

#endif