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
#include "beeper.h"
#include "fonts.h"

unsigned char roadbuf[200][250];
unsigned char *dst;
extern unsigned char road_color = ROAD_COLOR;
extern unsigned int y_line_counter=0;

//left_road=30, right_road=220 à la base.
unsigned short L_road[200], R_road[200];
unsigned short left_road, right_road;

int top_line = 0;   // la 1ère ligne visible

int scroll_speed = 0;
int y_offset_fp;
int counter_road_change = 0;
static void generate_line();



//static unsigned short grass_rng = 0xACE1;    utilisé pour bruit herbe mais trop lent

void create_tire_mark(char mark_color)
{

int i;
    int length;
    int x_start, x_end;
    int dx;
    int x;
    int acc;
    int step;
    int line;
    
    if (!(player.flags & PLAYER_ALIVE)) return;

    length = player.vy >> 4;
    if (length < 1)
        length = 1;

    x_start = player.box.x;
    x_end   = x_start + player.vx;

    dx = x_end - x_start;
    x = x_start;
    acc = 0;

    for (i = 0; i < length; i++)
    {
        line = (top_line + 188 - i + 200) % 200;
        dst = roadbuf[line];

        // traces de pneus
        memset(dst + x, mark_color, 2);
        memset(dst + x + 13, mark_color, 2);

        // incrément DDA
        acc += dx;
        if (length != 0)
        {
            step = acc / length;   //TODO enlever la division un jour
            x += step;
            acc -= step * length;
        }
    }

}

void gen_background()
{
	int i;
	
	for (i=0;i<200;i++)
	{
		dst = roadbuf[i];
		generate_line();
	}
}

/*static unsigned short lfsr16(void)
{
    grass_rng = (grass_rng >> 1) ^ (-(grass_rng & 1u) & 0xB400u);  Utilisé pour bruit herbe mais trop lent
    return grass_rng;
}*/

static void generate_line()
{    
	int i;
	unsigned char n;
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
    memset(dst, GRASS_COLOR , left_road);
    
    
    /*for (i = 0; i < left_road; i++)
{
    if ((lfsr16() & 63) == 0)   // ~1 sur 64  Utilisé pour bruit herbe mais trop lent
        dst[i] = 0x30;          // clair
    else
        dst[i] = 0x2F;          // normal
}*/
    
    memset(dst +left_road , ROAD_COLOR, 2);
    
    memset(dst +left_road +2 , LINE_COLOR, 5);


    // route
    memset(dst + left_road + 5, road_color, right_road - left_road - 5);
    
    
    
    memset(dst + right_road - 5 , LINE_COLOR, 3);
    
    memset(dst + right_road - 2 , ROAD_COLOR, 2);

    // herbe droite1
    memset(dst + right_road, GRASS_COLOR, 250 - right_road);
    
    /*for (int i = right_road; i < 250; i++)
{
    if ((lfsr16() & 63) == 0)    Utilisé pour bruit herbe mais trop lent
        dst[i] = 0x30;
    else
        dst[i] = 0x2F;
}*/

    // (ne surtout pas toucher à dst[250..319], c’est le HUD)
    
    dst[rand()&250]+=1;
    dst[rand()&250]+=1;
    
    if (y_line_counter!=0)
    	{
    	if (y_line_counter>0) y_line_counter--;
    	if (y_line_counter == 156) road_color = ROAD_COLOR;
    	if (y_line_counter == 1 && player.flags & PLAYER_ALIVE) 
    		{
    		init_fuel();
    		play_sound_id(SND_BONUS);
    		add_score(15);
    		new_level();
    		}
    	}
}




void init_background(){
	
	int i;
	top_line = 0;
	y_offset_fp = 0;
	road_change = 0;
	counter_road_change = 0;
	left_road=70;
	right_road=170;
	y_line_counter = 0;
	road_color = ROAD_COLOR;
	
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
