#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>
#include <dos.h>

#include "entity.h"
#include "background.h"
#include "control.h"
#include "enemy.h"
#include "sound.h"
#include "effects.h"
#include "decor.h"
#include "bullets.h"


int max_rel_x=30;

unsigned int collision_counter = 0;

int test_collision(entity *a, entity *b) {
    int ay, by, a2y, b2y, ax, bx, a2x, b2x;
    ay = a->y >> 4;
    by = b->y >> 4;
    a2y = ay+a->h;
    b2y = by+b->h;
    ax = a->x;
    bx=b->x;
    a2x=ax + a->w;
    b2x=bx + b->w;
    
    
    if (ax < b2x &&
        a2x > bx &&
        ay < b2y &&
        a2y > by)
    {
        return 1;
    }
    return 0;
}

void test_collision_bullet()
{

int i;

for (i=0;i<MAX_BULLET;i++)
	{
	if (!(bullet[i].flags & BULLET_ACTIVE)) continue;
	if (test_collision(&player.box, &bullet[i].box))
		{
		kill_player();
		bullet[i].flags=0;
		}
	}
}

void test_collision_bullet_civilian()
{

int i, j;
	for (j=0;j<MAX_BULLET;j++)
	{
		if (!(bullet[j].flags & BULLET_ACTIVE)) continue;
		for (i = j + 1; i < MAX_ENEMY; i++)
		{
			if (!(enemy[j].flags & ENEMY_ACTIVE) || enemy[j].type != CIVILIAN) continue;
			if (test_collision(&bullet[j].box, &enemy[j].box))
			{
				destroy_enemy(j);
				bullet[i].flags = 0;
			}
		}
	}
}

void test_collision_player_enemies(){
int i=0, rel_x, rel_y;

int dx, dx2, overlapX, dy, dy2, overlapY;

int push, cx, cy;


FILE *logf;

	if (collision_counter >0 ) collision_counter--;
	
	if (!player.alive) return;
	
	for (i=0; i<MAX_ENEMY; i++) {
    	if (!(enemy[i].flags & ENEMY_ACTIVE)) continue;
	
    	if (test_collision(&player.box, &enemy[i].box)) {
	
        	play_collision();
        	
        	if (enemy[i].flags & ENEMY_DEADLY) kill_player();
	
	    	dx  = (player.box.x + player.box.w) - enemy[i].box.x;
        	dx2 = (enemy[i].box.x + enemy[i].box.w) - player.box.x;
        	overlapX = dx < dx2 ? dx : dx2;
	
        	dy  = ((player.box.y>>4) + player.box.h) - (enemy[i].box.y>>4);
        	dy2 = ((enemy[i].box.y>>4) + enemy[i].box.h) - (player.box.y>>4);
        	overlapY = dy < dy2 ? dy : dy2;
	
        	//rel_x = player.vx - enemy[i].vx;
        	//rel_y = player.vy - enemy[i].vy;
        	
           	cx = (player.box.x + (player.box.w>>1))
           	- (enemy[i].box.x + (enemy[i].box.w>>1));
        	cy = ((player.box.y>>4) + (player.box.h>>1))
           	- ((enemy[i].box.y>>4) + (enemy[i].box.h>>1));
	
        	if (overlapX < overlapY) {
            	// choc horizontal
            	push = overlapX >> 1;
	
            	if (player.box.x>enemy[i].box.x) {
                	player.box.x += push;
                	enemy[i].box.x -= push;
                	
                	player.rvx += push;
                	enemy[i].rvx -= push<<1;
            	} else {
                	player.box.x -= push;
                	enemy[i].box.x += push;
                	
                	player.rvx -= push;
                	enemy[i].rvx += push<<1;
            	}
	
            	/*enemy[i].rvx = rel_x;  //player a forcément moins de recul que enemy, à corriger peut être
				player.rvx = -rel_x;
			
				enemy[i].rvy = rel_y<<1;
				player.rvy = -rel_y; */
        	} else {
            	// choc vertical
            	push = overlapY;
	
            	if (player.box.y>enemy[i].box.y) {
                	//player.box.y += push << 4;
                	enemy[i].box.y += push << 4;
                	
                	player.rvy -= push<<4;
                	enemy[i].rvy += push<<4;
            	} else {
                	//player.box.y -= push << 4;
                	enemy[i].box.y -= push << 4;
                	
                	player.rvy += push<<4;
                	enemy[i].rvy -= push<<4;
            	}
	
            	/*enemy[i].rvy = rel_y<<2;
				player.rvy = -rel_y<<1;
			
				enemy[i].rvx = rel_x>>1;
				player.rvx = -rel_x; */
     		}
     		
     		enemy[i].collision_timer=10;
		
    	}
	}
	
			
			
		
}

void test_collision_enemies_enemies(){
int i=0,j, rel_x, rel_y;

int dx, dx2, overlapX, dy, dy2, overlapY, ix, iy, jx, jy, iw, ih, jw, jh;

int push, cx, cy;

FILE *logf;

for (j=0;j<MAX_ENEMY;j++){
	if (!(enemy[j].flags & ENEMY_ACTIVE) || enemy[j].box.y<-560) continue;
	for (i = j + 1; i < MAX_ENEMY; i++) {
		
		if (!(enemy[i].flags & ENEMY_ACTIVE)) continue;
		
		jx = enemy[j].box.x;
		ix = enemy[i].box.x;
		jw = enemy[j].box.w;
		iw = enemy[i].box.w;
		
		if ( jx + enemy[j].box.w <= ix ) continue;
		if ( ix + enemy[i].box.w <= jx ) continue;
		//if ((unsigned)(jx - (ix + iw)) >= (unsigned)(jw + iw)) continue;

		
		jy = enemy[j].box.y>>4;
		iy = enemy[i].box.y>>4;
		jh = enemy[j].box.h;
		ih = enemy[i].box.h;
		
		//if ((unsigned)(jy - (iy + ih)) >= (unsigned)(jh + ih)) continue;
		if ( jy + enemy[j].box.h <= iy) continue;
		if ( iy + enemy[i].box.h <= jy) continue;
		
		//if (test_collision(&enemy[j].box, &enemy[i].box)) {

    		play_collision();
    		
    		dx  = (jx + jw) - ix;
    		dx2 = (ix + iw) - jx;
    		overlapX = dx < dx2 ? dx : dx2;
		
    		dy  = (jy + jh) - iy;
    		dy2 = (iy + ih) - jy;
    		overlapY = dy < dy2 ? dy : dy2;
		
    		if (overlapX < overlapY) 
    		{
        		// -----------------------
        		//     CHOC HORIZONTAL
        		// -----------------------
        		push = overlapX >> 1; // repousser de moitié
        		
		
        		if (jx>ix) {
            		// j est à droite
            		enemy[j].box.x += push;
            		enemy[i].box.x -= push;
            		
            		if (push>2) push=2;
            		enemy[j].rvx +=push<<1;
            		enemy[i].rvx -= push<<1;
        		} else {
            		// j est à gauche
            		enemy[j].box.x -= push;
            		enemy[i].box.x += push;
            		
            		if (push>2) push=2;
            		enemy[j].rvx -=push<<1;
            		enemy[i].rvx += push<<1;
        		}
				
				/*if (enemy[i].collision_timer==0){
    	    		//enemy[i].rvx =  rel_x;
	        		enemy[i].rvy =  rel_y;
        		}
        		
        		if (enemy[j].collision_timer==0){
        		//enemy[j].rvx = -rel_x << 1;
        		enemy[j].rvy = -rel_y;
        		} */
    		}
    		else {
        		// -----------------------
        		//       CHOC VERTICAL
        		// -----------------------
        		push = overlapY >> 1; // repousser de moitié
        				
        		if (jy>iy) {
            		// j est en bas
            		enemy[j].box.y -= push << 4;   // car tu stockes y <<4
            		enemy[i].box.y += push << 4;
            		
            		if (push>40) push=40;
            		enemy[j].rvy -=push<<5;
            		enemy[i].rvy += push<<5;
        		} else {
            		// j est en haut
            		enemy[j].box.y += push << 4;
            		enemy[i].box.y -= push << 4;
            		
            		if (push>40) push=40;
            		enemy[j].rvy +=push<<5;
            		enemy[i].rvy -= push<<5;
        		}
		
        		/*enemy[i].rvy =  rel_y << 2;
        		enemy[j].rvy = -rel_y << 1;
		
        		enemy[i].rvx =  rel_x >> 1;
        		enemy[j].rvx = -rel_x; */
    		}
    	//collision_counter+=5+(collision_counter>>1);
    	/*enemy[i].collision_timer=3;
    	enemy[j].collision_timer=3; */
		}					
	}
}
	


void test_collision_decor(){

int i;
    for (i=0;i<MAX_DECOR;i++) {
    	if (decor[i].flags & DECOR_ACTIVE && decor[i].flags & DECOR_SOLID){
    		if (test_collision(&player.box, &decor[i].box) && player.alive) kill_player();
    		}
    	}
}

void test_collision_decor_enemy(){

int i,j;


	for (j=0;j<MAX_ENEMY;j++){
		if (enemy[j].flags & ENEMY_ACTIVE && !(enemy[j].flags & ENEMY_INDESTRUCTIBLE)){
    		for (i=0;i<MAX_DECOR;i++) {
    			if (decor[i].flags & DECOR_ACTIVE && decor[i].flags & DECOR_SOLID){
    				if (test_collision(&enemy[j].box, &decor[i].box)) destroy_enemy(j);
    					
    			}
    		}
    	}
    }
}

void test_player_shoot(){
	int i, bottom, best_edge=-32768, best_id, touche=0;
	for (i=0;i<MAX_ENEMY;i++){
		if (enemy[i].flags & ENEMY_ACTIVE && !(enemy[i].flags & ENEMY_INDESTRUCTIBLE)){
			bottom = (enemy[i].box.y>>4)+enemy[i].box.h;
			if (player.box.x+7>enemy[i].box.x && player.box.x+7<enemy[i].box.x+enemy[i].box.w && player.box.y> bottom && bottom>0){
				if (bottom > best_edge) {
           			best_edge = bottom;
            		best_id = i;
            		touche=1;
					}
				
				}
			}	
		}
	if (touche) {
		spawn_effect(enemy[best_id].box.x, enemy[best_id].box.y+(enemy[best_id].box.h<<4), EFFECT_IMPACT);
		damage_enemy(GUN_DAMAGE,best_id);
		enemy[best_id].rvy+=55;
		}
	}
