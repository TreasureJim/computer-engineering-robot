#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

void SetError()
{
	PORTB |= 0b1 << PORTB2;
}

void ClearError()
{
	PORTB &= ~(0b1 << PORTB2);
}

void EndienSwap(void *data, uint8_t n)
{
	for (uint8_t i = 0; i < n / 2; i++)
	{
		uint8_t temp = *((uint8_t *)data + i);
		*((uint8_t *)data + i) = *((uint8_t *)data + n - i - 1);
		*((uint8_t *)data + n - i - 1) = temp;
	}
}

void BlinkLED()
{
	while (1)
	{
		SetError();
		_delay_ms(1000);
		ClearError();
		_delay_ms(1000);
	}
}