#include <pololu/3pi.h>

void follow_segment(/*int speed*/)
{
	int last_proportional = 0;
	long integral=0;

	while(1)
	{
		unsigned int sensors[5];
		read_line(sensors,IR_EMITTERS_ON);
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		
		int proportional = ((int)position) - 2000;

		
		int derivative = proportional - last_proportional;
		integral += proportional;

		
		last_proportional = proportional;


		/*const int max = 60; */
		
		int power_difference = proportional/20 + integral/10000 + derivative*3/2;

		// Compute the actual motor settings.  We never set either motor
		// to a negative value.
		const int max = 60/*speed*/; // the maximum speed
		if(power_difference > max)
			power_difference = max;
		if(power_difference < -max)
			power_difference = -max;
		
		if(power_difference < 0)
			set_motors(max+power_difference,max);
		else
			set_motors(max,max-power_difference);

		if(sensors[1] < 100 && sensors[2] < 100 && sensors[3] < 100)
		{
			return;
		}
		else if(sensors[0] > 200 || sensors[4] > 200)
		{
			return;
		}

	}
}

