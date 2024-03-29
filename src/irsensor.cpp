#include "irsensor.h"
#include "pins.h"
#include "motor.h"
#include <util/delay.h>
#include "bluetooth.h"
#include <stdio.h>
#include "helpers.h"

uint8_t IR_min = 0;
uint8_t IR_max = 255;

/// @brief Sensor initialization for the DDR and ADC.
void IR_InitialiseSensor()
{
	// Set the DDR as input for port C (analog pins)
	DDRC |= (0 << rightMidSensor) | (0 << leftMidSensor) | (0 << midMidSensor) | (0 << leftSensor) | (0 << rightSensor);
	// REFS0 - External Reference voltage (+5V)
	// ADLAR - Left shift the resulting ADC register (Only ADCH can be read)
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	// ADEN - Enable ADC
	// ADPS2:0 - Prescalar bits, set to 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/// @brief Reads the analog value on the analog pins
/// @param channel 0-4
/// @return Value of the ADCH register
uint8_t IR_ReadSensor(uint8_t channel)
{
	// Unsets the channel bits from previous call and preserve the upper 4 bits of the ADMUX register
	// 0x60 Keeps the REFS0 and ADLAR bits set (0110)
	ADMUX = (ADMUX & 0x60) | (channel & 0x0F);
	// Start ADC conversion
	ADCSRA |= (1 << ADSC);
	// Wait for the conversion to be done
	while (ADCSRA & (1 << ADSC))
		;
	;
	return ADCH;
}

/// @brief Calculates the minimum and maximum value of the sensors
/// This function does not return any values. Instead, the pointers
/// passed as parameters will be set outside this function.
/// @param min pointer to the min variable
/// @param max pointer to the max variable
void IR_GetLimits(uint8_t *min, uint8_t *max)
{
	uint8_t localMin = 255;
	uint8_t localMax = 0;
	for (int i = 0; i <= 300; i++)
	{
		uint8_t val1 = IR_ReadSensor(midMidSensor);
		_delay_ms(5);
		if (val1 < localMin)
		{
			localMin = val1;
		}
		else if (val1 > localMax)
		{
			localMax = val1;
		}
		uint8_t val2 = IR_ReadSensor(rightMidSensor);
		_delay_ms(5);
		if (val2 < localMin)
		{
			localMin = val2;
		}
		else if (val2 > localMax)
		{
			localMax = val2;
		}
		uint8_t val3 = IR_ReadSensor(leftMidSensor);
		_delay_ms(5);
		if (val3 < localMin)
		{
			localMin = val3;
		}
		else if (val3 > localMax)
		{
			localMax = val3;
		}
	}
	*min = localMin;
	*max = localMax;
}

///@brief Calculates the average value of the readings of the IR sensors
///@return Average value of sensors
uint8_t IR_GetAverageValue()
{
	uint8_t v1 = IR_ReadSensor(rightMidSensor);
	uint8_t v2 = IR_ReadSensor(leftMidSensor);
	uint8_t v3 = IR_ReadSensor(midMidSensor);
	return ((v1 + v2 + v3) / 3);
}

/// @brief Calculates a scaled value of the average value of all sensors.
/// getAverageValue() is called to get the values from the sensors and is
/// then scaled to a value between 0 and 1.
/// @param min min value of the average
/// @param max max value of the average
/// @return A float between 0 and 1 based on the given min and max values.
float IR_GetScaledValue(uint8_t *min, uint8_t *max)
{
	uint8_t avg = IR_GetAverageValue();
	return ((float)(avg - *min) / (float)(*max - *min));
}

/// @brief Calibrates the sensors to find mimimum and maximum value
/// @param min min value of sensors
/// @param max max value of sensors
void IR_CalibrateSensors(uint8_t *min, uint8_t *max)
{
	SetError();
	// start_motors();
	_delay_ms(1000);
	motorCalibration();
	uint8_t localMin;
	uint8_t localMax;
	IR_GetLimits(&localMin, &localMax);
	*min = localMin;
	*max = localMax;
	cut_motors();
	ClearError();

	char msg[30];
	uint8_t msg_size = sprintf(msg, "IR: min %x max %x\n", localMin, localMax);
	Bluetooth_Send(msg, msg_size + 1);
	_delay_ms(100);
}