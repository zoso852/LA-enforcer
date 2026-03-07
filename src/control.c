#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>
#include <dos.h>
#include <stdint.h>

#include "enforcer.h"
#include "display.h"
#include "control.h"
#include "entity.h"
#include "enemy.h"
#include "effects.h"
#include "level.h"
#include "background.h"
#include "collision.h"
#include "beeper.h"

#define BIOS_TICK  (*(volatile unsigned long far*)0x0040006CL)


unsigned long next_fire_tick = 0;

unsigned char cuda[530]={
15,36,2,4,29,6,5,30,11,12,7,23,11,30,34,1,218,254,1,1,29,2,9,30,11,2,242,13,2,238,15,4,242,19,2,241,21,1,29,22,3,242,25,3,30,28,3,29,31,3,30,34,2,242,254,0,1,29,1,2,30,3,5,29,8,3,30,11,2,242,13,2,28,15,3,29,18,3,30,21,1,29,22,3,242,25,1,238,26,1,30,27,1,29,28,6,30,34,1,182,35,1,242,254,0,1,29,1,9,30,10,3,242,13,1,28,14,2,29,16,5,30,21,2,242,23,1,238,24,1,21,25,1,242,26,8,30,34,1,182,35,1,242,254,0,10,30,10,3,242,13,1,29,14,1,30,15,3,26,18,3,30,21,2,242,23,1,238,24,1,21,25,1,242,26,8,30,34,1,242,35,1,238,254,0,10,30,10,2,242,12,1,241,13,1,30,14,3,27,17,1,26,18,3,30,21,2,242,23,1,238,24,1,21,25,1,242,26,8,30,34,2,242,254,0,6,78,6,9,23,15,1,21,16,1,27,17,1,26,18,3,78,21,2,238,23,2,21,25,1,238,26,8,78,34,2,238,254,0,6,30,6,1,22,7,8,25,15,1,21,16,1,27,17,1,26,18,3,30,21,3,238,24,1,21,25,1,242,26,8,30,34,2,242,254,0,6,78,6,9,22,15,1,21,16,1,27,17,1,26,18,3,78,21,2,242,23,2,21,25,1,242,26,8,78,34,1,242,35,1,238,254,0,10,30,10,1,242,11,1,238,12,1,242,13,1,30,14,3,27,17,1,26,18,3,30,21,2,242,23,1,238,24,1,21,25,1,242,26,8,30,34,1,242,35,1,238,254,0,10,30,10,2,242,12,1,238,13,2,30,15,3,26,18,3,30,21,2,242,23,1,238,24,1,21,25,1,242,26,8,30,34,1,242,35,1,238,254,0,10,30,10,2,242,12,1,238,13,8,30,21,2,242,23,2,21,25,1,242,26,8,30,34,1,182,35,1,238,254,0,3,30,3,5,29,8,3,30,11,2,242,13,9,30,22,4,242,26,3,30,29,4,29,33,1,30,34,1,182,35,1,238,254,1,10,30,11,1,238,12,2,242,14,5,241,19,2,242,21,1,30,22,2,218,24,1,242,25,9,30,34,1,242,35,1,238,254,2,9,27,11,14,26,25,9,27,34,1,242,255
};

unsigned char gunfire1[21]={
5,7,254,4,3,111,254,3,1,111,4,2,40,6,1,42,254,4,3,111,255
};

unsigned char gunfire2[43]={
5,7,3,3,111,254,2,1,111,3,3,40,6,1,111,254,0,2,111,2,2,40,4,2,42,6,1,43,254,2,1,111,3,3,40,6,1,111,254,3,3,111,255
};

unsigned char gunfire3[31]={
5,7,1,2,111,254,0,2,111,2,1,40,254,0,1,111,1,1,40,254,0,2,111,2,1,40,254,1,2,111,255
};

unsigned char* gunfire[3] = { gunfire1, gunfire2, gunfire3 };



#define MAX_VX 3    // vitesse x max
#define VX_ACC 3    // accélération par frame

unsigned char key_down[128];   // touche pressée
unsigned char key_up[128];     // touche relâchée

playertype player;


int v_slow=0;

int fuel_max =1600; //1600
int fuel_level=0;

int drift_counter = 0;

unsigned long game_over_tick = 0;  // moment où le joueur meurt
int game_over_delay = 48;          // nombre de ticks BIOS (~1/18.2 s par tick)
int game_over_active = 0;          // flag indiquant que l'écran de game over est affiché

extern char display_game_over=0;
	
static void player_undrift();

static void player_drift();

static void game_over_tick_on();

static void game_over();

void init_player(){

	player.flags = 0;
	player.box.x=150;
	
	player.box.y=157<<4;
	
	player.vx=0;
	player.vy=0;
	player.rvy=0;
	player.rvx=0;
	
	player.speed_index=0;
	
	player.sprite = cuda;
	
	player.box.w=player.sprite[0];
	player.box.h=player.sprite[1];
	player.flags |= PLAYER_ALIVE;
}

/* ---------------- Handler clavier ---------------- */
void interrupt (*old_keyboard_int)(void);  // ancien handler

void interrupt new_keyboard_int(void) {
    unsigned char sc = inp(0x60);
    int code = sc & 0x7F;

    if (sc & 0x80) { // release
        key_down[code] = 0;
        key_up[code] = 1;
    } else {         // press
        key_down[code] = 1;
        key_up[code] = 0;
    }

    outp(0x20, 0x20); // End of Interrupt
}

/* ---------------- Installation / restauration ---------------- */
void init_keyboard() {
    old_keyboard_int = _dos_getvect(0x09); // IRQ1
    _dos_setvect(0x09, new_keyboard_int);
}

void restore_keyboard() {
    _dos_setvect(0x09, old_keyboard_int);
}



void update_input() {

	short i;
	
	int offroad=0;
	int left_road_limit = (L_road[(top_line+(player.box.y>>4))%200])-3 ;
	int right_road_limit = (R_road[(top_line+(player.box.y>>4))%200])-13 ;
		
	FILE *logf;
	
	unsigned short acc_table[50] =
    {
    0, 1, 2, 2, 3, 4, 5, 7, 8, 10,
    12, 13, 15, 18, 20, 22, 25, 27, 30, 33,
    36, 39, 42, 45, 49, 53, 57, 61, 65, 68,
    73, 78, 82, 87, 91, 97, 101, 106, 110, 114,
    117, 120, 123, 125, 128, 129, 130, 131, 132, 133
};
		
	player.vx = 0;
    
    if (key_down[0x4B] && player.vy>0 && player.box.x>0)
    	{
        player.vx = -MAX_VX;
		if (player.flags & PLAYER_DRIFTING) player.vx = -MAX_VX - 2;
		}

    if (key_down[0x4D] && player.vy>0 && player.box.x<235)
    	{
        player.vx =  MAX_VX;
        if (player.flags & PLAYER_DRIFTING) player.vx = MAX_VX + 2;
        }
                        	
   	// Gestion friction
        
        if (player.rvx!=0) {
        	if (player.rvx>3||player.rvx<-3) {
        		int sign = (player.rvx > 0) ? 1 : -1;
    			player.rvx -= sign * ((abs(player.rvx) + 3) >> 2); // ~25% friction
    			}
        	if (player.rvx<0) player.rvx+=1;
        	if (player.rvx>0) player.rvx-=1;
		}

		if (player.rvy!=0)
			{
        	if (player.rvy<-3)
        		{
    			player.rvy -=  -((abs(player.rvy) + 3) >> 2); // ~25% friction
    			}
    		else if (player.rvy>3)
        		{
    			player.rvy -=  ((abs(player.rvy) + 3) >> 2); // ~25% friction
    			}
        	else if (player.rvy<0) player.rvy+=1;
        		else if (player.rvy>0) player.rvy-=1;
        	}
		
		/* if (player.rvy!=0)
			{
        	if (player.rvy>3||player.rvy<3)
        		{
        		int sign = (player.rvy > 0) ? 1 : -1;
    			player.rvy -= sign * ((abs(player.rvy) + 3) >> 2); // ~25% friction
    			}
        	else 
        		{ if (player.rvy<0) player.rvy+=1;  //TODO virer ces conditions si je suis sûr de garder -1 en friction
        		else {
        				if (player.rvy>0) player.rvy-=1;
        			 }
        		}
		} */
		
	
	
    // Accélération
        
    if (key_down[0x48])
    	{
    	if (player.flags & PLAYER_ALIVE && !(player.flags & PLAYER_OUT_OF_FUEL) && player.speed_index<49)
    		{
    		player.speed_index++;
    		if (player.vy < 40)
    			{
    			create_tire_mark(0xFF);
    			if (player.vy == 0) play_sound_id(SND_SQUEAL);
    			}
            }
        }
	else if (player.speed_index > 0)
		{
		player.speed_index--;
		
		}
	if (player.flags & PLAYER_ALIVE) player.vy_engine = acc_table[player.speed_index];
	/*
	// Accélération
    if (player.speed_index > 0) player.speed_index--;
    
    if (key_down[0x48] && player.flags & PLAYER_ALIVE && !(player.flags & PLAYER_OUT_OF_FUEL)) {
    
    		if (player.speed_index<49) player.speed_index++;

            player.vy_engine = acc_table[player.speed_index];
            
			}
	*/

    // Frein
    if (key_down[0x50] && player.flags & PLAYER_ALIVE) {
        	if(player.speed_index > 7) player.speed_index -= 2;
			if(player.speed_index < 8) player.speed_index = 0;

    		player.vy_engine = acc_table[player.speed_index];
    		
    		
    //On allume les feux arrières, ça c'est important
    	/*	cuda[63]=41;
    		cuda[94]=41;
    		cuda[418]=41;
    		cuda[396]=41;										TODO : A refaire quand le sprite Cuda sera définitif

        }
    if (key_up[0x50])  { //on les éteint
    		cuda[63]=182;
    		cuda[94]=182;
    		cuda[418]=182;
    		cuda[396]=182; */
    }
    
    // Tirer
     
     if (key_down[0x1D] && player.flags & PLAYER_ALIVE) {
     	player_fire();
     }
     
	// Drift, avec Alt 0x38
	
	if (key_down[0x38] && !(player.flags & PLAYER_DRIFTING) && player.vy>70) {
		player_drift();
		
	}
	
	if (key_up[0x38]) {
    	player_undrift();
	}
	
	
	
	     
     // Barre espace pour debug
     if (key_down[0x39]) 
     {
     	add_fuel(250);
     	//player.rvy+=50;
     	/*logf = fopen("log.txt", "a");
fprintf(logf, "scroll_y = %d \n", scroll_y);
fclose(logf); */
     } 
        
   // En dehors de la route? 
   
   
   if (player.box.x<left_road_limit || player.box.x>right_road_limit) 
   	{
   	offroad = 1;
   	if (player.box.x<left_road_limit-5 || player.box.x>right_road_limit+5) create_tire_mark(0xE7);
   	}
   
   /*
   //      OLD OFFRAOD
   	if (player.box.x<(L_road[(top_line+(player.box.y>>4))%200])-3 || player.box.x>(R_road[(top_line+(player.box.y>>4))%200])-13)
   		{
   		v_slow_target = player.vy_engine>>1;
   		//create_tire_mark(0x06);
   		}
       	else 
     		{
     		v_slow_target = 0;
     		while (v_slow!=0) v_slow -= 2;
     		}
     	
     if (v_slow<v_slow_target) v_slow+=2;
     */
     // Gestion drift
     
     if (player.flags & PLAYER_DRIFTING && drift_counter<12 &&  player.vy>70)
     	{
     	player.vy_engine -=20;
     	drift_counter++;
     	create_tire_mark(0xFF);
     	}
     
     
     player.vy = player.vy_engine + player.rvy - v_slow;
     
     if (offroad)
		{
    		// forte friction
    		player.vy -= player.vy >> 2;   // -25%
		
    		// vitesse max réduite
    		if (player.vy > 60)
        		player.vy = 60;
		
    		// JAMAIS de marche arrière
    		if (player.vy < 0)
        		player.vy = 0;
		}
         
     scroll_speed = player.vy;
     
     scroll_y+=(player.vy>>4);
     
     player.box.x+=player.vx+player.rvx;
     
     //Gestion fuel
     
     if (player.vy_engine>0)
     	{
     	fuel_level-=2;
        update_fuel_gauge(fuel_level>>5);
        }
        else player.vy_engine=0;
     
     if (fuel_level <= 0)
     	{
     	if (!(player.flags & PLAYER_OUT_OF_FUEL))
     		{
     		player.rvy+=player.vy_engine;
     		player.vy_engine = 0;
     		player.speed_index = 0;
     		player.flags |= PLAYER_OUT_OF_FUEL;
     		}
     	else if (player.vy <=0) game_over();
     	}
   	
   	
   	
   	draw_speed_cursor(scroll_speed/3);
        
     if (player.box.x>320) player.box.x-=320; // Utile tant qu'on wrappe autour de l'écran
     if (player.box.x<0) player.box.x+=320;
     
    //Gestion du game over
    
	if (game_over_active) {
	if ((int)(BIOS_TICK - game_over_tick) >= game_over_delay) game_over();
    }

          	
    
    memset(key_up, 0, sizeof(key_up));
	
}

void game_over()
{
	// On peut afficher le Game Over et permettre le restart
        display_game_over = 1;  // ta fonction pour l'affichage

        if (key_down[0x13]) {  // touche R
        	display_game_over = 0;
            init_game();
            game_over_active = 0;  // on reset le flag
            }
        if (key_down[0x10] || key_down[0x1E])
        	{
        	
        	 // attendre relâchement
    		while (key_down[0x10] || key_down[0x1E])
    			{
    			delay(1);
    			}
		
    		restore_keyboard();
		
    		// purge buffer BIOS
    		while (kbhit()) getch();
		
    		memset(key_down, 0, sizeof(key_down));
    		memset(key_up,   0, sizeof(key_up));
		
    		delay(50);
		
    		quit();
        			}
		}
		
void player_drift()
{
spawn_effect(player.box.x, player.box.y+450, EFFECT_SMOKE);
	play_sound_id(SND_SQUEAL);
	
    player.flags |= PLAYER_DRIFTING;
}
void player_undrift()
{
player.flags &= ~PLAYER_DRIFTING;
drift_counter = 0;
}

void player_fire(){




	//put_sprite(player.box.x, player.box.y, gunfire);
	
	// Vérifie si cooldown terminé
    if (BIOS_TICK >= next_fire_tick) {
		
        player.gun_timer = 3;
		play_sound_id(SND_SHOOT);
        // 2 tirs par seconde → 9 ticks BIOS
        next_fire_tick = BIOS_TICK + PLAYER_RELOAD_TIME;
        test_player_shoot();
    }
	
}

void player_gun_update(){
	if (player.gun_timer>0){
		put_sprite(player.box.x+5, player.box.y-1, gunfire[3-player.gun_timer]);
		player.gun_timer--;
	}
}

void stop_player()
{
	player.rvy += player.vy_engine;
	player.vy_engine = 0;
	player.speed_index = 0;
}

void kill_player(){
	//player.flags &= ~PLAYER_ALIVE;
	player.flags = 0;  //MET TOUS LES FLAGS A 0!!!
	
	player.rvy = player.vy_engine;
	player.vy_engine = 0;
	play_sound_id(SND_EXPLOSION);
	spawn_enemy(player.box.x,player.box.y,WRECK, WRECK_AI,player.vy, player.vx);
	spawn_effect(player.box.x-13, player.box.y-(8<<4), EFFECT_EXPLOSION);
	player.box.x = -340;
	player.box.y = 3600; //pour éviter les tests de collision
	
	game_over_tick_on();
	
}

void game_over_tick_on()
{
	// Démarrage du timer Game Over
    game_over_tick = BIOS_TICK;
    game_over_active = 1;
}

void init_fuel()
{
update_fuel_gauge(0);
fuel_level=fuel_max;
player.flags &= ~PLAYER_OUT_OF_FUEL;
update_fuel_gauge(fuel_level>>5);
}

void reduce_fuel(int amount)
{
	fuel_level-=amount;
	if (fuel_level<0) fuel_level = 0;
	update_fuel_gauge(fuel_level>>5);
}

void add_fuel(int amount)
{
	fuel_level+=amount;
	if (fuel_level>fuel_max) fuel_level = fuel_max;
	player.flags &= ~PLAYER_OUT_OF_FUEL;
	update_fuel_gauge(fuel_level>>5);
}
