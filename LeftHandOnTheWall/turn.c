#include <pololu/3pi.h>


void turn(char dir)
{
	switch(dir)
	{
	case 'L':
		set_motors(-80,80);
		delay_ms(200);
		break;
	case 'R':
		set_motors(80,-80);
		delay_ms(200);
		break;
	case 'B':
		set_motors(80,-80);
		delay_ms(400);
		break;
	case 'S':
		break;
	}
}
