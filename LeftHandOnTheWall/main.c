#include <avr/pgmspace.h>
#include <pololu/3pi.h>
#include "bargraph.h"
#include "maze-solve.h"

const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "Maze";
const char demo_name_line2[] PROGMEM = "solver";

const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";

void initialize()
{
	unsigned int mod;
	unsigned int sensors[5];

	
	pololu_3pi_init(2000);
	load_custom_characters();
	
	
	print_from_program_space(welcome_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(welcome_line2);
	play_from_program_space(welcome);
	delay_ms(1000);

	clear();
	print_from_program_space(demo_name_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(demo_name_line2);
	delay_ms(1000);

	
	while(!button_is_pressed(BUTTON_B))
	{
		int bat = read_battery_millivolts();

		clear();
		print_long(bat);
		print("mV");
		lcd_goto_xy(0,1);
		print("Press B");

		delay_ms(100);
	}

	
	wait_for_button_release(BUTTON_B);
	delay_ms(1000);

	
	for(mod=0;mod<80;mod++)
	{
		if(mod < 20 || mod >= 59)
		set_motors(40,-40);
		else
		set_motors(-40,40);

		calibrate_line_sensors(IR_EMITTERS_ON);

		delay_ms(17);
	}
	set_motors(0,0);

	while(!button_is_pressed(BUTTON_B))
	{
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		clear();
		print_long(position);
		lcd_goto_xy(0,1);
		display_readings(sensors);

		delay_ms(100);
	}
	wait_for_button_release(BUTTON_B);

	clear();

	print("Go!");

	play_from_program_space(go);
	while(is_playing());
}

int main()
{
	initialize();

	maze_solve();


	while(1);
}
