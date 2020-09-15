#include <pololu/3pi.h>
#include <string.h>
#include "follow-segment.h"
#include "turn.h"



char path[100] = "";
unsigned char path_length = 0;
unsigned char solved = 70;
unsigned char solving = 60;
void reverse(char *a)
{
	if (*a == 'R')
	*a = 'L';
	else if (*a == 'L')
	*a = 'R';
}

void change(char *a,int path_length)
{
	char t;
	for(int i = 0; i < path_length/2; i++){
		t = a[i];
		a[i] =a[path_length-i-1];
		reverse(&t);
		reverse(&a[i]);
		a[path_length-i-1] = t;
	}
	if(path_length%2 == 1)
		reverse(&a[path_length/2]);
}

void display_path()
{
	path[path_length] = 0;

	clear();
	print(path);

	if(path_length > 8)
	{
		lcd_goto_xy(0,1);
		print(path+8);
	}
}


char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
	if(found_left)
	return 'L';
	else if(found_straight)
	return 'S';
	else if(found_right)
	return 'R';
	else
	return 'B';
}

void simplify_path()
{
	
	
	if(path_length < 3 || path[path_length-2] != 'B')
	return;

	int total_angle = 0;
	int i;
	for(i=1;i<=3;i++)
	{
		switch(path[path_length-i])
		{
			case 'R':
			total_angle += 90;
			break;
			case 'L':
			total_angle += 270;
			break;
			case 'B':
			total_angle += 180;
			break;
		}
	}

	
	total_angle = total_angle % 360;

	
	switch(total_angle)
	{
		case 0:
		path[path_length - 3] = 'S';
		break;
		case 90:
		path[path_length - 3] = 'R';
		break;
		case 180:
		path[path_length - 3] = 'B';
		break;
		case 270:
		path[path_length - 3] = 'L';
		break;
	}
	path_length -= 2;
}

void maze_solve()
{
	while(1)
	{
		follow_segment(/*solving*/);

		
		set_motors(50,50);
		delay_ms(50);

		
		unsigned char found_left=0;
		unsigned char found_straight=0;
		unsigned char found_right=0;

		
		unsigned int sensors[5];
		read_line(sensors,IR_EMITTERS_ON);

		
		if(sensors[0] > 100)
			found_left = 1;
		if(sensors[4] > 100)
			found_right = 1;

		
		set_motors(40,40);
		delay_ms(200);
		
		read_line(sensors,IR_EMITTERS_ON);
		if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
			found_straight = 1;

		
		if(sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600)
			break;

		
		unsigned char dir = select_turn(found_left, found_straight, found_right);

		
		turn(dir);

		
		path[path_length] = dir;
		path_length ++;

		

		
		simplify_path();

		
		display_path();
	}

	
	while(1)
	{
		
		set_motors(0,0);
		play(">>a32");
		
		
		while(!button_is_pressed(BUTTON_B))
		{
			if(get_ms() % 2000 < 1000)
			{
				clear();
				print("Solved!");
				lcd_goto_xy(0,1);
				print("Press B");
			}
			else
			display_path();
			delay_ms(30);
		}
		while(button_is_pressed(BUTTON_B));
		
		delay_ms(1000);
		
		set_motors(80,-80);
		delay_ms(400);
		
		change(path,path_length);
		
		
		for(int i = 0; i < path_length; i++)
		{
			
			follow_segment(/*solved*/);

			
			set_motors(60,60);//aici am facut ultima modificare...Trebuie sa faci mai smooth curbele
			delay_ms(50);
			set_motors(40,40);
			delay_ms(200);
			
			
			turn(path[i]);
		}
		
		
		follow_segment(/*solved*/);

		
	}
}