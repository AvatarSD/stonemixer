/*
 * main.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: sd
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define INT0_DDR DDRD
#define INT0_PRT PORTD
#define INT0_PIN PIND
#define INT0_NUM 2

#define INT1_DDR DDRD
#define INT1_PRT PORTD
#define INT1_PIN PIND
#define INT1_NUM 3

#define OUT0_DDR DDRD
#define OUT0_PRT PORTD
#define OUT0_PIN PIND
#define OUT0_NUM 4

#define OUT1_DDR DDRD
#define OUT1_PRT PORTD
#define OUT1_PIN PIND
#define OUT1_NUM 5

#define PERIOD 	 60

void setStopped()
{
	OUT0_PRT |= _BV(OUT0_NUM);
	OUT1_PRT |= _BV(OUT1_NUM);
}

void setR()
{
	OUT0_PRT |= _BV(OUT0_NUM);
	OUT1_PRT &= ~_BV(OUT1_NUM);
}

void setL()
{
	OUT0_PRT &= ~_BV(OUT0_NUM);
	OUT1_PRT |= _BV(OUT1_NUM);
}

void setDead()
{
	OUT0_PRT &= ~_BV(OUT0_NUM);
	OUT1_PRT &= ~_BV(OUT1_NUM);
}

void turnPin(uint16_t count)
{
	count *= 2;
	for (; count > 0; count--)
	{
		OUT0_PRT ^= _BV(OUT0_NUM);
		_delay_us(1500);
	}
}

volatile uint16_t sensorTime = 0;
volatile uint16_t periodLastTime = 0;
volatile bool lastErr = false;

inline void doCheckpoint()
{
	periodLastTime = sensorTime;
	sensorTime = 0;
}

ISR(TIMER1_CAPT_vect)
{
	static uint16_t sec = PERIOD;
	if (sec++ == PERIOD)
	{
		sec = 0;

		if (periodLastTime == 0)
			if (lastErr)
				setDead();
			else
				setStopped();
		else
		{
			uint16_t checkCount = (PERIOD * 10) / periodLastTime;
			if (checkCount >= 10)
				turnPin(checkCount);

		}
	}

	if (sensorTime <= PERIOD)
		sensorTime++;
	else
		setStopped();
}

volatile bool err = false;

ISR(INT0_vect)
{
	if ((INT1_PIN & _BV(INT1_NUM)))
	{
		doCheckpoint();
		if (err)
		{
			lastErr = true;
			setDead();
		}
		err = true;
	}
	else
	{
		err = false;
		lastErr = false;
		setR();
	}
}

ISR(INT1_vect)
{
	if ((INT0_PIN & _BV(INT0_NUM)))
	{
		doCheckpoint();
		if (err)
		{
			lastErr = true;
			setDead();
		}
		err = true;
	}
	else
	{
		err = false;
		lastErr = false;
		setL();
	}
}

void init()
{
	INT0_DDR &= ~_BV(INT0_NUM);
	INT0_PRT &= ~_BV(INT0_NUM);
	INT1_DDR &= ~_BV(INT1_NUM);
	INT1_PRT &= ~_BV(INT1_NUM);
	OUT0_DDR |= _BV(OUT0_NUM);
	OUT0_PRT |= _BV(OUT0_NUM);
	OUT1_DDR |= _BV(OUT1_NUM);
	OUT1_PRT |= _BV(OUT1_NUM);

	EICRA = (1 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
	EIMSK = (1 << INT1) | (1 << INT0);
	//EIFR = (1 << INTF1) | (1 << INTF0);

	TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0)
			| (0 << WGM11) | (0 << WGM10);
	TCCR1B = (0 << ICNC1) | (0 << ICES1) | (1 << WGM13) | (1 << WGM12)
			| (1 << CS12) | (0 << CS11) | (0 << CS10);
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	ICR1H = 0xF4;
	ICR1L = 0x23;
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	OCR1BH = 0x00;
	OCR1BL = 0x00;
	TIMSK1 = (1 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (0 << TOIE1);

}

int main()
{
	init();

	sei();

	while (true)
	{

	}

	return 0;
}

