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
#include "level.h"

unsigned char roadbuf[200][250];
unsigned char *dst;

//left_road=30, right_road=220 à la base.
unsigned short L_road[200], R_road[200];
unsigned short left_road=70, right_road=170;

int top_line = 0;   // la 1ère ligne visible

int scroll_speed = 0;
int y_offset_fp;
int counter_road_change = 0;
static void generate_line();

void gen_background()
{
	int i;
	FILE *logf;
	
	/*logf = fopen("log.txt", "a");
	fprintf(logf, "left_road from gen_background = %d ,", left_road);
	fclose(logf); */
	
	for (i=0;i<200;i++)
	{
		dst = roadbuf[i];
		generate_line();
	}
}

static void generate_line()
{
    FILE *logf;
    
    /*logf = fopen("log.txt", "a");
	fprintf(logf, "counter_road_change = %d \n", counter_road_change);
	fclose(logf);  */
    
    switch (road_change)
    {
    
    	
    	case TURN_LEFT :
     	if (counter_road_change>2)
     		{
     		left_road--;
    		right_road--;
    		counter_road_change=0;
    		}
    		else counter_road_change++;
    	break;
    	
    	case TURN_RIGHT :
    	if (counter_road_change>2)
     		{
     		left_road++;
    		right_road++;
    		counter_road_change = 0;
    		}
    		else counter_road_change++;
    	break;
    	
    	
		case NARROW_LEFT:
		if (counter_road_change>2)
			{
			left_road++;
			counter_road_change=0;
			}
			else counter_road_change++;
		break;
			
		case NARROW_RIGHT:
		if (counter_road_change>2)
			{
			right_road--;
			counter_road_change=0;
			}
			else counter_road_change++;
		break;
		
		case NARROW_CENTER :
		if (counter_road_change>2)
			{
			right_road--;
			left_road++;
			counter_road_change = 0;
			}
			else counter_road_change++;
		break;
			
		case EXPAND_LEFT :
		if (counter_road_change>2)
			{
			left_road--;
			counter_road_change = 0;
			}
			else counter_road_change++;
		break;
		
		case EXPAND_RIGHT :
		if (counter_road_change>2)
			{
			right_road++;
			counter_road_change = 0;
			}
			else counter_road_change++;
		break;
    }
    
    
    L_road[top_line] = left_road;
    R_road[top_line] = right_road;
    // herbe gauche
    memset(dst, 0x2F, left_road);
    
    memset(dst +left_road , 0x17, 2);
    
    memset(dst +left_road +2 , 0x0F, 5);


    // route
    memset(dst + left_road + 5, 0x17, right_road - left_road - 5);
    
    
    
    memset(dst + right_road - 5 , 0x0F, 3);
    
    memset(dst + right_road - 2 , 0x17, 2);

    // herbe droite1
    memset(dst + right_road, 0x2F, 250 - right_road);

    // (ne surtout pas toucher à dst[250..319], c’est le HUD)
}


void init_background(){
	
	int i;
	top_line = 0;
	y_offset_fp = 0;
	
	for (i=0;i<200;i++)
		{
		L_road[i]=left_road;
		R_road[i]=right_road;
		}
    	
	gen_background();
		
}


void update_background()
{
	 int i;
	 int line_height_fp; 

	 FILE *logf;
	line_height_fp = 1 << 4;
    y_offset_fp += scroll_speed;

/*logf = fopen("log.txt", "a");
fprintf(logf, "scrollspeed = %d , y_offset_fp = %d , line_height_fp = %d\n", scroll_speed, y_offset_fp, line_height_fp);
fclose(logf); */

    while (y_offset_fp >= line_height_fp)
    {
        y_offset_fp -= line_height_fp;
        
        // avancer l'index
        top_line--;
        if (top_line < 0)
            top_line = 199;
        
        
        
        // écrire la nouvelle ligne dans la ligne la plus haute
        dst = roadbuf[top_line];
        generate_line();

        
    }
}
