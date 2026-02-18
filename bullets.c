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
#include "sound.h"
#include "bullets.h"

unsigned char regular_bullet [70] =
{
 5,5,1,1,40,2,1,41,3,1,40,254,0,1,40,1,1,67,2,1,68,3,1,67,4,1,40,254,0,1,41,1,1,68,2,1,15,3,1,68,4,1,41,254,0,1,40,1,1,67,2,1,68,3,1,67,4,1,40,254,1,1,40,2,1,41,3,1,40,255
};

bullet_t bullet[MAX_BULLET];

BulletType bullet_types[1] = {
    {regular_bullet, 4}, // chiffre = vitesse
};

void init_bullets(void)
{
    int i;
    for (i = 0; i < MAX_BULLET; ++i)
    {
        bullet[i].flags = 0;
       
        bullet[i].vx = bullet[i].vy = 0;
    }
}

void shoot_bullet (int enemy_number)
{
	
	int ax,bx,ay,by,dx,dy,absx,absy, d, i;
	
	ax = player.box.x;
	ay = player.box.y>>4;
	
	bx = enemy[enemy_number].box.x;
	by = enemy[enemy_number].box.y>>4;
	
	dx = ax-bx;
	dy = ay-by;
	
	absx = abs(dx);
	absy = abs(dy);
	
	d = absx>absy ? absx : absy;
		
	for (i=0; i<MAX_BULLET;i++)
		{
		if (!(bullet[i].flags & BULLET_ACTIVE)) break; 
		}
		
	bullet[i].flags |= BULLET_ACTIVE;
	
	bullet[i].type = REGULAR_BULLET;
	
	bullet[i].box.w = bullet_types[bullet[i].type].sprite[0];
	bullet[i].box.h = bullet_types[bullet[i].type].sprite[1];
	
	bullet[i].box.x = enemy[enemy_number].box.x;
	bullet[i].box.y = enemy[enemy_number].box.y;
	
	bullet[i].vx = (dx*bullet_types[bullet[i].type].speed)/d;
	bullet[i].vy = (dy*bullet_types[bullet[i].type].speed)/d;
}

void update_bullets()
{
	int i;
	
	for (i=0; i<MAX_BULLET;i++)
		{
		if (!(bullet[i].flags & BULLET_ACTIVE)) continue;
		if (bullet[i].box.x<0 || bullet[i].box.x>250 || bullet[i].box.y < 0 || bullet[i].box.y >3185) bullet[i].flags = 0;
		
		bullet[i].box.x += bullet[i].vx;
		bullet[i].box.y += (bullet[i].vy<<4); //+ scroll_speed;
		
		
		
		put_sprite(bullet[i].box.x,bullet[i].box.y, bullet_types[bullet[i].type].sprite);
		}
}
