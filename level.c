#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>

#include "display.h"
#include "background.h"
#include "control.h"
#include "decor.h"
#include "level.h"
#include "enemy.h"

char road_change=0;
FILE *logf;

long scroll_y=0;
int interval = 0;
int old_value;

void create_decor()
{
	int x, road_offset;
		
	if (rand()%20==1 && nb_decor<MAX_DECOR)
	{
		road_offset = right_road-left_road;
		
		x= rand()%(250-road_offset);
		
		if(x>left_road) x+=road_offset;
			else x-= decor_types[PALM_TREE].sprite[0];
		
		gen_decor(x,-70, PALM_TREE);
	}
}

void create_enemy()
	{
		if (rand()%150==1 && nb_enemy<MAX_ENEMY)
		spawn_enemy(170, -70<<4, VIPER, STANDART_AI, 120,0);
	}

void create_civilian()
	{
		int lane;
		if (rand()%75==1 && nb_enemy<MAX_ENEMY)
		{
			lane=(rand()%3)+1;
			spawn_enemy(right_road-(28*lane), -60<<4, CIVILIAN, CIVILIAN_AI, 60,lane);
		}
	}

void create_road()
{
	int counter, random;
	

	interval += (scroll_speed>>4);
	


	if (interval > 200) 
		{
		interval-=200;
		random = rand()%7;
		
		//random = 6;
		switch (random)
		{
			case 0 :
			road_change=TURN_LEFT;
			break;
			case 1 :
			road_change=TURN_RIGHT;
			break;
			
			case 2 :
			if (right_road-left_road>110)
				{
				road_change=NARROW_LEFT;
				old_value = left_road;
				}
			break;
			
			case 3 :
			if (right_road-left_road>110)
				{
				road_change=NARROW_RIGHT;
				old_value = right_road;
				}
			break;
			
			case 4 :
			if (right_road-left_road>130)
				{
				road_change=NARROW_CENTER;
				old_value = right_road;
				}
			break;
			
			case 5 :
			if (left_road>50)
				{
				road_change=EXPAND_LEFT;
				old_value = left_road;
				}
			break;
			
			case 6 :
			if (right_road<191)
				{
				road_change=EXPAND_RIGHT;
				old_value = right_road;
				}
			break;
				
		}
		}
		switch (road_change)
		{
			case TURN_LEFT :
			if (left_road<30) road_change=0;
					
			break;
					
			case TURN_RIGHT :
			if (right_road>220) road_change=0;
			break;
			
			case NARROW_LEFT :
			if (left_road-old_value>30) road_change =0;
			break;
			
			case NARROW_RIGHT :
			if (old_value-right_road>30) road_change =0;
			break;
			
			case NARROW_CENTER :
			if (old_value-right_road>15) road_change =0;
			break;
			
			case EXPAND_LEFT :
			if (old_value-left_road>30) road_change =0;
			break;
			
			case EXPAND_RIGHT :
			if (right_road-old_value>30) road_change =0;
			break;
		}
		
	
	
}
