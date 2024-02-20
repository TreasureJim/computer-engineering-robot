#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

//PORT C, pin 0
#define right_sensor PINC0
//PORT C, pin 1
#define left_sensor PINC1
//PORT C, pin 2
#define mid_sensor PINC2

void initalize_sensors();

#endif