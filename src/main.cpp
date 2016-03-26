/*
 * main.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: sd
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define INT0_DDR DDRD
#define INT0_PRT PORTD
#define INT0_PIN PIND
#define INT0_NUM 2

#define INT1_DDR DDRD
#define INT1_PRT PORTD
#define INT1_PIN PIND
#define INT1_NUM 3

ISR(INT0_vect)
{

}

ISR(INT1_vect)
{

}

void init()
{


}

int main()
{
	init();

	while(true)
	{

	}

	return 0;
}

