#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>

#include "display.h"
#include "enemy.h"
#include "control.h"
#include "effects.h"
#include "background.h"
#include "beeper.h"
#include "bullets.h"
#include "decor.h"
#include "level.h"
#include "fonts.h"

unsigned char far viper[631] = {
17,39,5,1,114,6,1,115,7,4,137,11,1,115,12,7,114,19,8,185,27,7,228,254,1,1,115,2,36,232,254,0,1,114,1,1,64,2,1,65,3,3,64,6,1,12,7,1,64,8,3,42,11,1,41,12,1,232,13,1,41,14,1,136,15,1,18,16,4,209,20,6,136,26,1,232,27,11,42,38,1,232,254,0,1,232,1,1,163,2,1,64,3,1,42,4,7,41,11,1,113,12,1,19,13,1,136,14,1,41,15,3,139,18,1,138,19,3,232,22,1,138,23,3,137,26,13,41,254,0,1,232,1,1,163,2,1,12,3,7,232,10,1,41,11,1,18,12,1,19,13,1,210,14,8,12,22,3,42,25,2,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,232,1,1,64,2,8,41,10,1,232,11,1,18,12,1,19,13,1,137,14,1,12,15,12,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,12,1,1,65,2,8,41,10,1,232,11,1,18,12,1,19,13,1,138,14,1,12,15,12,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,64,1,1,65,2,8,41,10,1,232,11,1,18,12,1,19,13,1,163,14,1,42,15,12,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,64,1,1,65,2,8,41,10,1,232,11,1,18,12,1,19,13,1,232,14,13,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,64,1,1,65,2,8,41,10,1,232,11,1,18,12,1,19,13,1,232,14,13,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,12,1,1,65,2,8,41,10,1,232,11,1,18,12,1,19,13,1,232,14,1,42,15,12,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,232,1,1,65,2,1,42,3,8,41,11,1,18,12,1,19,13,1,137,14,1,42,15,12,41,27,1,232,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,232,1,1,164,2,1,12,3,8,41,11,1,208,12,1,19,13,1,210,14,1,42,15,12,41,27,1,136,28,1,19,29,1,18,30,1,136,31,8,41,254,0,1,232,1,1,163,2,1,12,3,8,41,11,1,113,12,1,19,13,1,136,14,1,232,15,10,42,25,4,41,29,2,232,31,8,41,254,0,1,114,1,1,12,2,2,64,4,1,12,5,5,42,10,3,41,13,1,136,14,1,209,15,10,19,25,1,136,26,12,42,38,1,232,254,1,1,113,2,10,232,12,4,113,16,21,232,37,1,112,254,4,1,185,5,1,114,6,5,137,11,1,113,12,7,114,19,5,185,24,6,228,30,4,227,255
};

unsigned char far wreck[639]={
16,35,14,1,18,15,2,17,254,15,4,17,19,1,183,20,1,17,23,1,17,254,12,1,17,13,1,207,14,1,184,15,3,17,18,1,208,19,2,183,23,1,17,25,1,17,254,7,5,18,12,1,17,13,2,18,15,2,183,17,3,17,23,1,17,26,1,208,27,2,17,29,3,208,32,1,17,254,0,1,18,1,1,232,2,1,207,3,1,17,4,8,18,13,2,18,15,1,184,16,1,183,17,1,184,18,1,17,23,1,18,24,1,17,26,1,18,27,2,17,29,2,18,31,1,183,32,1,208,33,1,207,254,0,1,18,1,4,17,5,7,18,12,1,17,13,1,185,14,1,184,15,2,208,17,1,18,22,1,17,23,1,18,24,1,17,26,2,18,28,1,17,29,1,18,30,2,207,32,1,183,33,1,18,34,1,17,254,0,1,18,1,1,17,2,2,18,4,1,208,5,7,18,12,1,17,13,1,18,14,1,184,15,1,18,16,3,208,19,1,18,23,1,18,24,1,17,26,4,18,30,2,207,32,1,18,33,1,183,34,1,17,254,0,1,18,1,1,183,2,3,208,5,1,18,6,1,207,7,1,232,8,1,208,9,3,18,13,1,18,15,2,17,17,2,18,19,1,17,20,3,18,23,1,17,26,1,18,27,1,183,28,1,208,29,1,18,30,2,183,32,3,18,254,0,1,17,1,1,185,2,1,184,3,1,183,4,2,208,6,1,18,7,2,17,9,3,18,14,6,18,20,1,17,22,3,18,26,1,18,27,2,183,29,1,18,30,1,183,31,2,208,33,1,17,34,1,18,254,0,1,17,1,1,185,2,2,184,4,1,208,5,1,183,6,3,18,9,1,208,10,2,18,13,1,17,14,1,18,15,1,17,22,1,18,24,4,18,28,4,208,32,1,183,33,2,18,254,0,1,17,1,1,185,2,1,184,3,1,183,4,1,185,5,2,208,7,1,209,8,1,232,9,3,209,12,1,208,13,3,17,20,1,17,21,8,18,29,1,208,30,2,207,32,1,208,33,2,18,254,0,1,17,1,1,184,2,1,183,3,1,184,4,1,17,5,3,18,8,5,17,13,1,207,14,7,18,21,11,17,32,3,18,254,0,3,18,3,7,17,10,2,183,12,1,17,13,1,18,14,1,208,15,2,17,17,1,18,18,15,17,33,2,18,254,0,2,18,2,6,17,8,1,18,9,2,208,11,1,183,12,1,184,13,1,183,14,1,18,15,1,17,16,2,183,18,14,17,32,1,18,33,1,207,34,1,232,254,0,1,17,1,4,18,5,4,17,9,1,18,10,1,183,11,2,208,13,1,18,14,1,17,15,3,18,18,1,208,19,7,17,27,4,17,31,1,18,32,1,17,254,5,1,18,11,3,18,14,1,208,15,1,207,16,1,18,17,1,208,18,6,17,29,1,17,30,1,18,255
};

unsigned char far civilian [834] = {
19,39,4,34,17,254,1,3,17,4,6,229,10,2,227,12,26,113,38,1,17,254,0,1,17,1,1,228,2,2,229,4,6,113,10,3,17,13,1,208,14,1,17,15,5,208,20,1,17,21,2,208,23,10,17,33,1,113,34,1,114,35,3,113,38,1,17,254,0,1,17,1,8,113,9,1,16,10,1,209,11,1,207,12,1,209,13,12,17,25,3,18,28,2,209,30,5,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,4,113,5,4,232,9,1,17,10,1,208,11,2,17,13,7,232,20,4,41,24,3,42,27,4,17,31,1,207,32,1,17,33,1,232,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,208,11,1,17,12,14,113,26,1,185,27,2,209,29,1,207,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,208,11,1,17,12,14,113,26,1,17,27,2,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,207,11,1,17,12,14,113,26,1,17,27,2,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,207,11,1,17,12,14,113,26,1,17,27,2,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,209,11,1,17,12,14,113,26,1,17,27,2,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,209,11,1,17,12,14,113,26,1,17,27,2,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,209,11,1,17,12,14,113,26,1,17,27,1,207,28,1,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,209,11,1,17,12,14,113,26,1,17,27,1,207,28,1,209,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,209,11,1,17,12,14,113,26,1,17,27,2,207,29,1,208,30,1,209,31,1,207,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,18,11,1,17,12,13,114,25,1,113,26,1,17,27,2,208,29,1,207,30,1,209,31,1,208,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,8,113,9,1,17,10,1,18,11,20,17,31,1,208,32,1,17,33,1,113,34,1,17,35,1,113,36,1,232,37,1,113,38,1,17,254,0,1,17,1,9,113,10,1,17,11,3,208,14,1,17,15,5,208,20,1,17,21,6,208,27,4,232,31,4,17,35,1,113,36,1,232,37,1,113,38,1,17,254,1,3,17,4,6,113,10,2,227,12,20,113,32,1,114,33,5,113,38,1,17,254,4,34,17,255
};

EnemyType enemy_types[] = {
    { 20, viper, 0 },
	{ 20, wreck, 0 },
	{ 15, civilian,0}
};

enemy_t enemy[MAX_ENEMY];

int nb_enemy=0;
int nb_civilian=0;
int y_target_counter = 0;

int shoot_rand;

void init_enemy(){
	int i;

	for (i=0; i<MAX_ENEMY;i++)
		{
		enemy[i].flags = 0;
		}
	nb_enemy=0;
	nb_civilian=0;
	shoot_rand = 40;
};


void spawn_enemy(int x, int y, int type, char ai, int speed, int vx) {
    int i;
    for (i = 0; i < MAX_ENEMY; i++) {
        if (!(enemy[i].flags & ENEMY_ACTIVE)) {

			enemy[i].flags = 0;
            enemy[i].flags |= ENEMY_ACTIVE;
            
            enemy[i].type = type;
            if (enemy[i].type==WRECK)
            	{
            	enemy[i].flags |= ENEMY_DEADLY;
            	enemy[i].flags |= ENEMY_INDESTRUCTIBLE;
            	}
            
            enemy[i].hp = enemy_types[enemy[i].type].max_hp;

            enemy[i].box.x = x;
            enemy[i].box.y = y;
            
            enemy[i].collision_timer=0;
            
            
            enemy[i].box.w = enemy_types[enemy[i].type].sprite[0];
            enemy[i].box.h = enemy_types[enemy[i].type].sprite[1];
            

            enemy[i].ai = ai;

            enemy[i].vy_engine = speed;
            enemy[i].vx=vx;
            
            enemy[i].rvx = 0;
            enemy[i].rvy = 0;
            
            if (ai == STANDART_AI)
            	{
            	nb_enemy++;
            	if (y_target_counter == 0) 
            		{
            		enemy[i].y_target = 15<<4;	//Hauteur 1
            		y_target_counter = 1;
            		}
            	else 
            		{
            		enemy[i].y_target = 50<<4;	//hauteur 2
            		y_target_counter = 0;
            		}
            	}
            else if (ai == CIVILIAN_AI)
            	{
            	enemy[i].vx_target = vx ;// = lane (1 ou 2)
            	nb_civilian++;
                }   
            enemy[i].vx_target=E_MAX_SPEED;
            
             
            
            
            enemy[i].vx_steer=0;
            enemy[i].collision_timer=0;


            return;
        }
    }
}



void update_enemies(){
	int i;
	signed int y_diff;
	FILE *logf;
	
    for (i = 0; i < MAX_ENEMY; i++) {
    
    if (enemy[i].flags & ENEMY_ACTIVE){
        
        // On détruit les enemy trop loins.
        if (enemy[i].box.y < -1600 || enemy[i].box.y > 6400)
        	{
        	enemy[i].flags = 0;
        	if (enemy[i].ai==CIVILIAN_AI)
        		{
        		nb_civilian--;
        		civilian_counter_rand-=CIVILIAN_COUNTER_RAND;
        		}
        	else if (enemy[i].ai == STANDART_AI)
        		{
        		nb_enemy--;
        		enemy_counter_rand-=enemy_counter_rand_delta;
        		}
        	}
        
        if (enemy[i].collision_timer>0) enemy[i].collision_timer--;
        
        //***************************STANDART AI******************************
        
        if (enemy[i].ai == STANDART_AI)
        	{        
        	//Gestion vitesse
       		
       		if (enemy[i].box.y > 0)
       			{
       			y_diff = enemy[i].box.y-enemy[i].y_target;
       			enemy[i].vy_engine+=y_diff>>8;
       			if (enemy[i].vy_engine>150) enemy[i].vy_engine =150;
       			else if (enemy[i].vy_engine<105) enemy[i].vy_engine =105;
       			}
        	
        	// Gestion tir
        	
        	if (rand()%shoot_rand == 0 && nb_bullet<max_bullet_level) shoot_bullet(i);
        	
        	//mouvement aléatoire X
        	
        	
        	if (rand() % 20 == 0)
        		{      // 1 chance sur 20 par frame
            	enemy[i].vx_target = -enemy[i].vx_target;
            	       	
        		}
        	if (enemy[i].box.x> (right_road-40) ) enemy[i].vx_target = -abs(enemy[i].vx_target);
        	if (enemy[i].box.x< (left_road+30) ) enemy[i].vx_target = abs(enemy[i].vx_target);
        	
        	
	
        	if (enemy[i].vx_steer < enemy[i].vx_target)
        		{
            	enemy[i].vx_steer += E_ACCEL;
            	if (enemy[i].vx_steer > enemy[i].vx_target) enemy[i].vx_steer = enemy[i].vx_target;
        		}
        	else if (enemy[i].vx_steer > enemy[i].vx_target)
        		{
            	enemy[i].vx_steer -= E_ACCEL;
            	if (enemy[i].vx_steer < enemy[i].vx_target) enemy[i].vx_steer = enemy[i].vx_target;
        		}
        	}
        
         enemy[i].vx = enemy[i].vx_steer + enemy[i].rvx;
        
        //*********************************WRECK AI************************************
        
        if (enemy[i].ai == WRECK_AI)
        	{
        		if (enemy[i].vy_engine > 7) enemy[i].vy_engine -= 7;
        			else enemy[i].vy_engine = 0;
        		if (enemy[i].vx > 0) enemy[i].vx--;
				if (enemy[i].vx < 0) enemy[i].vx++;
        			
        		
        		       
        	}
        //*****************************CIVILIAN AI*******************************
        if (enemy[i].ai == CIVILIAN_AI)
        	{
        		//logf = fopen("log.txt", "a");
//fprintf(logf, "AVANT AI : x= %d vx= %d rightroad-40 = %d \n", enemy[i].box.x, enemy[i].vx, right_road-40); */

        		if (enemy[i].box.x<R_road[(top_line-0+(enemy[i].box.y>>4))%200]-(28*enemy[i].vx_target) )
        			{
        			enemy[i].vx = 1;
        			}
        		else if (enemy[i].box.x>R_road[(top_line-0+(enemy[i].box.y>>4))%200]-(28*enemy[i].vx_target) ) enemy[i].vx = -1;
        		
        		        		
        		//fprintf(logf, "topline = %d, y = %d, topline(y) = %d\n", top_line,enemy[i].box.y>>4, (top_line+(enemy[i].box.y>>4))%200);
//fclose(logf);
        	}
        
        // Gestion friction
        
        if (enemy[i].rvx!=0) {
        	if (enemy[i].rvx > 0) enemy[i].rvx--;
				if (enemy[i].rvx < 0) enemy[i].rvx++;
    			}
        	else enemy[i].rvx = 0;
        }
        
        
        if (enemy[i].rvy!=0)
			{
        	if (enemy[i].rvy<-3)
        		{
    			enemy[i].rvy -=  -((abs(enemy[i].rvy) + 3) >> 2); // ~25% friction
    			}
    		else if (enemy[i].rvy>3)
    			{
    			enemy[i].rvy +=  -((abs(enemy[i].rvy) + 3) >> 2); // ~25% friction
    			}
        	else if (enemy[i].rvy<0) enemy[i].rvy+=1;
        		else if (enemy[i].rvy>0) enemy[i].rvy-=1;
        	}
        
        /*
        //   OLD FRICTION
        if (enemy[i].rvy!=0) {
        	if (enemy[i].rvy>3||enemy[i].rvy<3) {
        		sign = (enemy[i].rvy > 0) ? 1 : -1;
    			enemy[i].rvy -= sign * ((abs(enemy[i].rvy) + 3) >> 2); // ~25% friction
    			}
        	else enemy[i].rvy = 0;
        	

				}
        */
        enemy[i].vy = enemy[i].vy_engine+enemy[i].rvy;
        
        // Gestion Y suivant vitesse du joueur
        
        enemy[i].box.y+=scroll_speed-enemy[i].vy;
        
        enemy[i].box.x+=enemy[i].vx+enemy[i].rvx;
        
        
        
        
	}
}

void draw_enemies() {
    int i;
    for (i = 0; i < MAX_ENEMY; i++) {
        if (enemy[i].flags & ENEMY_ACTIVE) {

            enemy_t *e = &enemy[i];   // raccourci

            // récupère le sprite à partir du type
            unsigned char far *spr = enemy_types[e->type].sprite;

            // dessine
            put_sprite(e->box.x, e->box.y, spr);
        }
    }
}

void damage_enemy (int damage, int enemy_number){
	enemy[enemy_number].hp-=damage;
	if (enemy[enemy_number].hp<0)
		{
		if (enemy[enemy_number].ai == CIVILIAN_AI)
			{
			reduce_fuel(100); //Si c'est un civilian, malus
			add_score(-5);
			}
		else 
			{
			add_score(5);  //sinon bonus
			}
		destroy_enemy(enemy_number);
		}
}

void destroy_enemy(int enemy_number) {

if (enemy[enemy_number].ai == CIVILIAN_AI)
	{
	play_sound_id(SND_MALUS);
	nb_civilian--;
	civilian_counter_rand-=CIVILIAN_COUNTER_RAND;
	}
else if (enemy[enemy_number].ai == STANDART_AI)
	{
	play_sound_id(SND_EXPLOSION);
	gen_decor(enemy[enemy_number].box.x,enemy[enemy_number].box.y>>4,JERRYCAN);
	nb_enemy--;
	enemy_counter_rand-=enemy_counter_rand_delta;
	if (nb_enemy==0) enemy_counter_rand=0; // Si le joueur change de niveau, le dellta change parce qu'il n"est pas propre à chaque enemy. Donc tac solution pourrie.
	}

spawn_enemy(enemy[enemy_number].box.x,enemy[enemy_number].box.y,WRECK, WRECK_AI,enemy[enemy_number].vy,enemy[enemy_number].vx);

enemy[enemy_number].flags = 0;
spawn_effect(enemy[enemy_number].box.x-13,enemy[enemy_number].box.y-(8<<4),EFFECT_EXPLOSION);

}
