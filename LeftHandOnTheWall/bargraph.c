#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const char levels[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};


void load_custom_characters()
{
	lcd_load_custom_character(levels+0,0); 
	lcd_load_custom_character(levels+1,1); 
	lcd_load_custom_character(levels+2,2); 
	lcd_load_custom_character(levels+3,3);
	lcd_load_custom_character(levels+4,4);
	lcd_load_custom_character(levels+5,5);
	lcd_load_custom_character(levels+6,6);
	clear();
}

void display_readings(const unsigned int *calibrated_values)
{
	unsigned char i;

	for(i=0;i<5;i++) {
		const char display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};

		char c = display_characters[calibrated_values[i]/101];

		print_character(c);
	}
}
