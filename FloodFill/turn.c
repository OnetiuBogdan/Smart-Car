/*
 * turn.c
 *
 * Created: 29-Aug-20 11:44:48 PM
 *  Author: sbogd
 */ 

#include <pololu/3pi.h>

void turn(int miscare)
{
	switch(miscare)
	{
		case 3://stanga
		set_motors(-80,80);
		delay_ms(200);
		break;
		case 1://dreapta
		set_motors(80,-80);
		delay_ms(200);
		break;
		case 2://inapoi
		set_motors(80,-80);
		delay_ms(400);
		break;
		case 0:
		break;
	}
}
