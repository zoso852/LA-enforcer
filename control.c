#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>
#include <dos.h>
#include <stdint.h>

#include "display.h"
#include "control.h"
#include "entity.h"
#include "enemy.h"
#include "effects.h"
#include "level.h"
#include "background.h"
#include "sound.h"
#include "collision.h"
#include "beeper.h"

#define BIOS_TICK  (*(volatile unsigned long far*)0x0040006CL)

unsigned long next_fire_tick = 0;

unsigned char cuda[470]={
15,36,2,9,30,11,12,15,23,11,30,34,1,218,254,1,10,30,11,2,242,13,2,238,15,4,242,19,2,241,21,1,30,22,3,242,25,9,30,34,2,242,254,0,11,30,11,2,242,13,9,30,22,3,242,25,1,238,26,8,30,34,1,182,35,1,242,254,0,10,30,10,3,242,13,8,30,21,2,242,23,1,238,24,1,8,25,1,242,26,8,30,34,1,182,35,1,242,254,0,10,30,10,3,242,13,2,30,15,3,7,18,3,30,21,2,242,23,1,238,24,1,8,25,1,242,26,8,30,34,1,242,35,1,238,254,0,10,30,10,2,242,12,1,241,13,1,30,14,3,27,17,1,7,18,3,30,21,2,242,23,1,238,24,1,8,25,1,242,26,8,30,34,2,242,254,0,6,78,6,9,23,15,1,8,16,1,27,17,1,7,18,3,78,21,2,238,23,2,8,25,1,238,26,8,78,34,2,238,254,0,6,30,6,1,22,7,8,25,15,1,8,16,1,27,17,1,7,18,3,30,21,3,238,24,1,8,25,1,242,26,8,30,34,2,242,254,0,6,78,6,9,22,15,1,8,16,1,27,17,1,7,18,3,78,21,2,242,23,2,8,25,1,242,26,8,78,34,1,242,35,1,238,254,0,10,30,10,1,242,11,1,238,12,1,242,13,1,30,14,3,27,17,1,7,18,3,30,21,2,242,23,1,238,24,1,8,25,1,242,26,8,30,34,1,242,35,1,238,254,0,10,30,10,2,242,12,1,238,13,2,30,15,3,7,18,3,30,21,2,242,23,1,238,24,1,8,25,1,242,26,8,30,34,1,242,35,1,238,254,0,10,30,10,2,242,12,1,238,13,8,30,21,2,242,23,2,8,25,1,242,26,8,30,34,1,182,35,1,238,254,0,11,30,11,2,242,13,9,30,22,4,242,26,8,30,34,1,182,35,1,238,254,1,10,30,11,1,238,12,2,242,14,5,241,19,2,242,21,1,30,22,2,218,24,1,242,25,9,30,34,1,242,35,1,238,254,2,9,27,11,14,7,25,9,27,34,1,242,255};

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

int fuel_level = 1600;
	

void init_player(){
	
	player.box.x=150;
	
	player.box.y=157<<4;
	
	player.vx=0;
	player.vy=0;
	
	player.speed_index=0;
	
	player.sprite = cuda;
	
	player.box.w=player.sprite[0];
	player.box.h=player.sprite[1];
	player.alive=1;
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
	
	int v_slow_target = 0;
	
	FILE *logf;
	
	unsigned short acc_table[50] = /*{0,1,2,3,4,5,7,9,11,13,15,17,20,23,26,29,32,35,39,43,47,51,55,59,64,69,74,79,84,89,95,101,107,113,119,126,132,138,143,148,152,156,160,163,166,168,170,171,172,173}; */
    {
    0, 1, 2, 2, 3, 4, 5, 7, 8, 10,
    12, 13, 15, 18, 20, 22, 25, 27, 30, 33,
    36, 39, 42, 45, 49, 53, 57, 61, 65, 68,
    73, 78, 82, 87, 91, 97, 101, 106, 110, 114,
    117, 120, 123, 125, 128, 129, 131, 131, 132, 133
};
	if (key_up[0x4B]) player.vx = 0;
	if (key_up[0x4D]) player.vx = 0;
    
    if (key_down[0x4B])
        player.vx = -MAX_VX;

    if (key_down[0x4D])
        player.vx =  MAX_VX;
                        	
   	// Gestion friction
        
        if (player.rvx!=0) {
        	if (player.rvx>3||player.rvx<3) {
        		int sign = (player.rvx > 0) ? 1 : -1;
    			player.rvx -= sign * ((abs(player.rvx) + 3) >> 2); // ~25% friction
    			}
        	if (player.rvx<0) player.rvx+=1;
        	if (player.rvx>0) player.rvx-=1;
		}

		if (player.rvy!=0) {
        	/* logf = fopen("log.txt", "a");
fprintf(logf, "player rvy = %d \n ", player.rvy);
fclose(logf); */
        	if (player.rvy>3||player.rvy<3) {
        		int sign = (player.rvy > 0) ? 1 : -1;
    			player.rvy -= sign * ((abs(player.rvy) + 3) >> 2); // ~25% friction
    			}
        	else { if (player.rvy<0) player.rvy+=1;
        		else {
        				if (player.rvy>0) player.rvy-=1;
        			}
        		}
		}
		
	

    // Accélération
    if (key_down[0x48] && player.alive) {
    
    		if (player.speed_index<49) player.speed_index++;

            player.vy_engine = acc_table[player.speed_index];
            
            fuel_level-=2;
            update_fuel_gauge(fuel_level>>5);
			}

    // Frein
    if (key_down[0x50] && player.alive) {
        	if(player.speed_index > 7) player.speed_index -= 2;
			if(player.speed_index < 8) player.speed_index = 0;

    		player.vy_engine = acc_table[player.speed_index];
    		
    		
    //On allume les feux arrières, ça c'est important
    		cuda[63]=41;
    		cuda[94]=41;
    		cuda[418]=41;
    		cuda[396]=41;

        }
    if (key_up[0x50])  {
    		cuda[63]=182;
    		cuda[94]=182;
    		cuda[418]=182;
    		cuda[396]=182;
    }
        
   // En dehors de la route? 
   
   	if (player.box.x<(L_road[(top_line+(player.box.y>>4))%200])-3 || player.box.x>(R_road[(top_line+(player.box.y>>4))%200])-13)
   		{
   		//play_collision();
   		v_slow_target = player.vy_engine>>1;
   		}
     //if (player.box.x<left_road || player.box.x>right_road) v_slow_target = 150;
     	else 
     		{
     		v_slow_target = 0;
     		while (v_slow!=0) v_slow -= 2;
     		}
     	
     if (v_slow<v_slow_target) v_slow+=2;
        
     player.vy = player.vy_engine + player.rvy - v_slow;
     
     scroll_speed = player.vy;
     
     scroll_y+=((player.vy_engine-v_slow)>>4);
     
     player.box.x+=player.vx+player.rvx;
   	
   	
   	/*logf = fopen("log.txt", "a");
fprintf(logf, "player speed = %d \n", playerspeed);
fclose(logf); */
   	
   	draw_speed_cursor(scroll_speed/3);
        
     if (player.box.x>320) player.box.x-=320; // Utile tant qu'on wrappe autour de l'écran
     if (player.box.x<0) player.box.x+=320;
     
     // Tirer
     
     if (key_down[0x1D]) {
     	player_fire();
     }
     	
     if (key_down[0x39]) 
     {
     	play_test();
     }
    
	
}

void player_fire(){




	//put_sprite(player.box.x, player.box.y, gunfire);
	
	// Vérifie si cooldown terminé
    if (BIOS_TICK >= next_fire_tick) {
		
        player.gun_timer = 3;
		play_sound_id(SND_SHOOT);
        // 2 tirs par seconde → 9 ticks BIOS
        next_fire_tick = BIOS_TICK + 4;
        test_player_shoot();
    }
	
}

void player_gun_update(){
	if (player.gun_timer>0){
		put_sprite(player.box.x+5, player.box.y-1, gunfire[3-player.gun_timer]);
		player.gun_timer--;
	}
}

void kill_player(){
	player.alive=0;
	//scroll_speed=0;
	player.rvy = player.vy_engine;
	player.vy_engine = 0;
	play_sound_id(SND_EXPLOSION);
	spawn_effect(player.box.x-13, player.box.y-(8<<4), EFFECT_EXPLOSION);
	player.box.x = -340;
	player.box.y = 3600; //pour éviter les tests de collision
	//spawn_wreck(player.box.x, player.box.y, player.vy);
	//put_sprite (player.box.x,player.box.y,wreck);
}

void reduce_fuel(int amount)
{
	fuel_level-=amount;
	update_fuel_gauge(fuel_level);
}
