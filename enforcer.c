#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>
#include <time.h>
#include <dos.h>

#include "display.h"
#include "enemy.h"
#include "background.h"
#include "control.h"
#include "entity.h"
#include "collisio.h"
#include "sound.h"
#include "effects.h"
#include "decor.h"
#include "level.h"
#include "bullets.h"

int mescouilles=0;

unsigned char car[883]={
115,3,
21,50,
6,1,17,7,1,211,8,2,136,10,2,137,12,2,136,14,2,114,16,14,185,30,1,17,34,1,185,35,1,211,36,1,210,37,1,233,38,1,209,39,2,18,41,1,17,
2,2,113,4,2,115,6,37,6,43,3,115,46,2,113,48,1,186,
1,1,6,2,3,12,5,3,6,8,5,42,13,4,41,17,1,6,18,1,113,19,1,6,20,1,115,21,1,136,22,10,6,32,16,41,48,1,6,49,1,17,
0,1,18,1,2,64,3,4,65,7,3,64,10,4,42,14,1,41,15,1,209,16,1,6,17,1,41,18,1,6,19,15,19,34,1,6,35,14,42,49,1,41,
0,1,186,1,1,12,2,1,24,3,1,42,4,10,41,14,1,113,15,1,18,16,1,19,17,1,234,18,1,42,19,4,12,23,10,6,33,17,41,
0,1,136,1,1,12,2,1,24,3,1,41,4,9,6,13,1,41,14,2,18,16,1,19,17,1,234,18,1,12,19,1,64,20,9,12,29,1,42,30,1,12,31,2,42,33,2,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,6,1,1,64,2,1,12,3,11,41,14,2,18,16,1,19,17,1,137,18,1,12,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,6,1,1,65,2,1,64,3,10,41,13,1,6,14,2,18,16,1,19,17,1,6,18,1,12,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,42,1,1,65,2,1,12,3,10,41,13,1,6,14,2,18,16,1,19,17,1,12,18,1,42,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,12,1,1,65,2,1,12,3,10,41,13,1,6,14,2,18,16,1,19,17,1,12,18,1,42,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,12,1,1,65,2,1,12,3,10,41,13,1,6,14,2,18,16,1,19,17,1,12,18,17,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,64,1,1,65,2,1,12,3,10,41,13,1,6,14,2,18,16,1,19,17,1,42,18,17,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,12,1,1,65,2,1,64,3,10,41,13,1,6,14,2,18,16,1,19,17,1,12,18,1,42,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,6,1,2,65,3,11,41,14,2,18,16,1,19,17,1,6,18,1,42,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,6,1,1,65,2,1,64,3,11,41,14,2,18,16,1,19,17,1,137,18,1,42,19,16,41,35,1,136,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,115,1,1,164,2,1,12,3,11,41,14,1,208,15,1,18,16,1,19,17,1,234,18,1,42,19,16,41,35,1,231,36,2,19,38,1,18,39,1,6,40,10,41,
0,1,186,1,1,12,2,1,24,3,1,42,4,8,6,12,2,41,14,1,6,15,1,18,16,1,19,17,1,234,18,1,6,19,13,42,32,4,41,36,2,6,38,1,136,39,1,6,40,10,41,
0,1,185,1,1,42,2,1,24,3,2,65,5,2,12,7,3,42,10,5,41,15,3,6,18,1,136,19,13,234,32,1,136,33,3,41,36,13,42,49,1,41,
1,1,6,2,8,42,10,1,6,11,3,42,14,1,41,15,1,6,16,1,209,17,4,208,21,12,209,33,1,6,34,3,42,37,2,6,39,1,41,40,4,42,44,5,41,49,1,17,
2,2,113,4,1,115,5,38,6,43,3,115,46,2,113,48,1,185,
6,1,185,7,1,210,8,1,136,9,4,137,13,1,136,14,1,114,15,1,186,16,9,185,33,1,17,34,1,186,35,4,209,39,2,18,41,1,17
};

volatile unsigned long timer_ticks = 0;
unsigned long last_tick = 0;
void interrupt (*old_timer_isr)();

static void interrupt new_timer_isr() {
    timer_ticks++;

    // appeler l'ancien handler (BIOS timer)
    old_timer_isr();

    // envoyer EOI
    outp(0x20, 0x20);
}

static void install_timer() {
    _disable();

    old_timer_isr = _dos_getvect(0x08);
    _dos_setvect(0x08, new_timer_isr);

    _enable();
}

static void set_fps(int fps) {
    unsigned divisor = 1193180 / fps;

    outp(0x43, 0x36);         // PIT channel 0, mode 3 (square wave)
    outp(0x40, divisor & 0xFF);
    outp(0x40, divisor >> 8);
}



int main() {
    
    int i;
    FILE *logf;
    
    srand(time(NULL));
       
    opl_init();
    init_display();
    init_background();
    init_player();
    init_keyboard();
    init_effects();
    init_sound();
    
    //init_bullets();
    
    init_decor();
	
	set_fps(30); //18.2, 25, 30, 40, 50, 60, 70, 75, 100, 120
    install_timer();
    
    clear_buffer();
    
    while(1) {
    
    if ( player.vy == 173 && mescouilles == 0) { 
    	//spawn_enemy(130,60 <<4,VIPER,STANDART_AI,160,0);
    	//spawn_enemy(150, 90<<4, CIVILIAN, CIVILIAN_AI, 60,0);
    	
    	mescouilles=1;
    	}
    	if (timer_ticks != last_tick) {
        last_tick = timer_ticks;
    	
    update_input();          // lire touches
    
    
    test_collision_decor();
    test_collision_decor_enemy();
    
    update_enemies();
    
    test_collision_player_enemies();
    test_collision_enemies_enemies();
    test_collision_bullet();
    test_collision_bullet_civilian();
    
    
    
    
    
    update_display();
    
    create_road();
    
    create_decor();
    
    create_enemy();
    
    create_civilian();
    
    }
    
    update_sounds();
    
	update_background();
    
    update_decor();
    
    update_effects();
    
    if (player.alive == 1) put_sprite (player.box.x,player.box.y,player.sprite);
    
    player_gun_update();
    
    draw_enemies();
    
    draw_effects();
    
    update_bullets();
    
    
    
	flip();
	//delay(3);
	
	}

    
    getch(); /* Attendre une touche avant de quitter */
    restore_keyboard();
    opl_stop();
    sound_shutdown();
    free_buffer();
    return 0;
}
