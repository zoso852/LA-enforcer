#ifndef CONTROL_H
#define CONTROL_H

/*#define ACC_CURVE 5
#define BRAKE 7
#define MAX_SPEED 500 */

#define GUN_DAMAGE 5
#define PLAYER_RELOAD_TIME 4 //4

#define PLAYER_ALIVE 0x01
#define PLAYER_DRIFTING 0x02
#define PLAYER_OUT_OF_FUEL 0x04

#include "entity.h"

typedef struct {
    entity box;
    int vx, vy, vy_engine;
    int rvx, rvy;
    unsigned char far *sprite;
    short speed_index, steering;
    char gun_timer;
    unsigned char flags;
    
} playertype;

extern playertype player;
extern char keyboard_ready;
extern char display_game_over;

void init_keyboard();
void restore_keyboard();
void init_player();
void update_input();
void player_fire();
void player_gun_update();
void kill_player();
void init_fuel();
void reduce_fuel(int amount);
void add_fuel(int amount);

#endif
