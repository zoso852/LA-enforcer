#ifndef CONTROL_H
#define CONTROL_H

#define ACC_CURVE 5
#define BRAKE 7
#define MAX_SPEED 500

#define GUN_DAMAGE 5

#include "entity.h"

typedef struct {
    entity box;
    int vx, vy, vy_engine;
    int rvx, rvy;
    unsigned char far *sprite;
    short speed_index, steering;
    char gun_timer;
    char alive;
    
} playertype;

extern playertype player;
extern char keyboard_ready;

void init_keyboard();
void restore_keyboard();
void init_player();
void update_input();
void player_fire();
void player_gun_update();
void kill_player();
void reduce_fuel(int amount);

#endif
