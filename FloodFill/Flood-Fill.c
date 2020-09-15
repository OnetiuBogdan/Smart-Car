/*
 * Flood_Fill.c
 *
 * Created: 29-Aug-20 11:43:35 PM
 *  Author: sbogd
 */ 


#include <stdio.h>
#include <stdlib.h>
#include <pololu/3pi.h>
#include "follow-segment.h"
#include "turn.h"


//Macrouri
#define max_row 15
#define max_col 20

int matrix[max_row][max_col] = {0};
	
//Variabile
int rows = 2, cols = 0;
int move = 0, k = 0, wasBack = 0;
unsigned int sensors[5];
int direction = 1;
long start_time, time_travel, time_need = 390;
/** 
    direction * 0 = nord
    direction * 1 = east
    direction * 2 = south
    direction * 3 = west
*/
void move_by_direction();
void straight();
void rotate_right();
void rotate_back();
void rotate_left();
int counter();
int select_turn();
void check_next();
void set_direction();
void display_path();
void display_path1();
int select_turn_back();
void back();
//char vector[40] = "";
int c = 0;
//int t = 0;

void flood_fill()
{ 
	//long start_time = get_ms();
	while(1) 
	{
	
		start_time = get_ms(); //- start_time;
		
		follow_segment();

		set_motors(50,50);
		delay_ms(50);
		
		time_travel = get_ms() - start_time;
		
		for(int i = 1; i < time_travel/time_need;i++)
		{
			move_by_direction();
			//display_path();
		}
		
		unsigned int dreapta = 0, stanga = 0, inainte = 0;
		
		read_line(sensors,IR_EMITTERS_ON);
		
		if(sensors[0] > 100)
			stanga = 1;
		if(sensors[4] > 100)
			dreapta = 1;

		set_motors(40,40);
		delay_ms(200);
		
		read_line(sensors,IR_EMITTERS_ON);
		if(sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200)
		{
			inainte = 1;
		}

		if(sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600)
		{
			break;
		}

		int way = select_turn(stanga,inainte,dreapta);
		
		turn(way);
		
		//display_path();
		move_by_direction();		
		//display_path();
		move = 0;
		
		if(wasBack == 1)
		{
			check_next();
		}
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
			//display_path1();
			delay_ms(30);
		}
		while(button_is_pressed(BUTTON_B));
		
		delay_ms(1000);
		
		set_motors(80,-80);
		delay_ms(400);
		
		direction = (direction + 2) % 4;
		
		back();
		break;		
	}
	
}

void move_by_direction()//Suntem in est
{
		
    if(move == 3)//stanga
    {
        rotate_left();
    }
    else if(move == 1)//dreapta
    {
        rotate_right();
    }
    else if(move == 2)//spate
    {
		rotate_back();
    }
    else if(move == 0)//fata
    {
        straight();
    }
}

int counter()
{
	if (wasBack) {
		k--;
		return 255;
	}

	k++;
	return k;
}

void rotate_left()
{
	direction = (direction - 1 + 4) % 4;
}

void rotate_right()
{
	direction = (direction + 1 + 4) % 4;
}

void rotate_back()
{
	direction = (direction + 2 + 4) % 4;
	matrix[rows][cols] = 255;
	wasBack = 1;
}

void straight()
{
	switch(direction)
	{
		case 0://N
			rows -= 1;
			break;

		case 1://E
			cols += 1;
			break;

		case 2://S
			rows += 1;
			break;

		case 3://W
			cols -= 1;
			break;

		default:
			break;
	}
	
	matrix[rows][cols] = counter();
	/*if(t < 34)
	{vector[t++] = 'a' + rows;
	vector[t++] = 'a' + cols;} 
	display_path();*/
}

int select_turn(int stanga,int inainte,int dreapta)
{
	if(stanga == 1)
	{
		move = 3;
	}
	else if(inainte == 1)
	{
		move = 0;
	}
	else if(dreapta == 1)
	{
		move = 1;
	}
	else
	{ 
		move = 2;
	}
	return move;
}

void check_next() {
	switch (direction) 
	{
		case 0:
		if (matrix[rows - 1][cols] == 0)
		{
			wasBack = 0;
			matrix[rows][cols] = counter();
		}
		break;

		case 1:
		if (matrix[rows][cols + 1] == 0)
		{
			wasBack = 0;
			matrix[rows][cols] = counter();
		}
		break;

		case 2:
		if (matrix[rows + 1][cols] == 0)
		{
			wasBack = 0;
			matrix[rows][cols] = counter();
		}
		break;

		case 3:
		if (matrix[rows][cols - 1] == 0)
		{
			wasBack = 0;
			matrix[rows][cols] = counter();
		}
		break;
	}
}

/*void display_path(long b)
{
	
	clear();
	print_long(b);
	if(b > 8)
	{
		lcd_goto_xy(0,1);
		print_long(b+8);
	}
}*/

/*void display_path1()
{
	int m = 0;
	while(m < (t - (t % 16))){
	
	clear();
	print(vector + m);
	m += 8;
	lcd_goto_xy(0,1);
	print(vector + m);
	delay_ms(5000);
	lcd_goto_xy(0,0);
	m += 8;
	}
}*/


void display_path()
{	
	print_long(rows);
	print_long(cols);
}


void back()
{
	while(1)
	{
		follow_segment();//Merge pana la prima intersectie

		set_motors(50,50);
		delay_ms(50);

		int way = select_turn_back();//decide calea pe care o va urma in intersectie
		
		turn(way);//executa virajul
		
		move_by_direction();//modifica directia in memorie
		
		if(rows == 2 && cols == 0)// 2 si 0 coordonate inceput
		{
			break;
		}
	}
	
	
	while(1)//Pe baza matrici o sa mearga de inceput la sfarsit si invers
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
			//display_path1();
			delay_ms(30);
		}
		while(button_is_pressed(BUTTON_B));
		
		delay_ms(1000);
		
		set_motors(80,-80);
		delay_ms(400);
		
		direction = (direction + 2) % 4;
		
		back();
		
	}
	
}


int select_turn_back()//Verificarea miscarii pe baza directiei si a matricii in intersectie 
{
	switch (direction)
	{
		case 0://Nord
		if (matrix[rows - 1][cols] > 0 && matrix[rows - 1][cols] < 255 )
		{
			move = 0;//Inainte
		}
		
		if (matrix[rows][cols + 1] > 0 && matrix[rows][cols + 1] < 255)
		{
			move = 1;//Dreapta
		}
		
		if (matrix[rows][cols - 1] == 0 && matrix[rows][cols - 1] < 255)
		{
			move = 3;//Stanga
		}
		break;



		case 1://Est
		if (matrix[rows - 1][cols] > 0 && matrix[rows - 1][cols] < 255 )
		{
			move = 3;//Stanga
		}
		
		if (matrix[rows][cols + 1] > 0 && matrix[rows][cols + 1] < 255)
		{
			move = 0;//Inainte
		}
		
		if (matrix[rows + 1][cols] > 0 && matrix[rows + 1][cols] < 255)
		{
			move = 1;//Dreapta
		}
		break;

		
		
		case 2://Sud
		if (matrix[rows][cols + 1] > 0 && matrix[rows][cols + 1] < 255)
		{
			move = 3;//Stanga
		}
		
		if (matrix[rows + 1][cols] > 0 && matrix[rows + 1][cols] < 255)
		{
			move = 0;//Inainte
		}
		
		if (matrix[rows][cols - 1] == 0 && matrix[rows][cols - 1] < 255)
		{
			move = 1;//Dreapta
		}
		break;

		
		
		case 3://Vest
		if (matrix[rows - 1][cols] > 0 && matrix[rows - 1][cols] < 255 )
		{
			move = 1;//Dreapta
		}
		
		if (matrix[rows + 1][cols] > 0 && matrix[rows + 1][cols] < 255)
		{
			move = 3;//Stanga
		}
		
		if (matrix[rows][cols - 1] == 0 && matrix[rows][cols - 1] < 255)
		{
			move = 0;//Inainte
		}
		break;
	}
	return move;
}