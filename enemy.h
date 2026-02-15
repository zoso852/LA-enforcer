#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

#define E_MAX_SPEED 2
#define E_ACCEL 1

#define MAX_ENEMY 10
#define MAX_CIVILIAN 16

#define ENEMY_ACTIVE 0x01  // 0000 0001
#define ENEMY_SOLID  0x02  // 0000 0010
#define ENEMY_VISIBLE 0x04
#define ENEMY_DEADLY 0x08  //collision mortelle
#define ENEMY_INDESTRUCTIBLE 0x16 // utile pour les wrecks

typedef struct {
    int max_hp;
    unsigned char far *sprite;
    int speed;
} EnemyType;

#define VIPER 0
#define WRECK 1
#define CIVILIAN 2

typedef struct {
	entity box;
    int vx, vy, vy_engine, vx_steer; //vx = vitesse totale (vx_steer+rvx), vy = vy_engine+rvy
    int rvx, rvy; //vitesse de rebond
    int collision_timer;
    int vx_target;  //=lane pour les civilian
    int hp;
    unsigned char flags;
    char ai;
    int type;    // index dans enemy_types[]
} enemy_t;

#define WRECK_AI 1
#define CIVILIAN_AI 2
#define STANDART_AI 3

extern enemy_t enemy[MAX_ENEMY];
extern int nb_enemy;

void init_enemy();
void spawn_enemy(int x, int y, int type, char ai,int speed,int vx);
void update_enemies();
void draw_enemies();
void damage_enemy(int damage, int enemy_number);
void destroy_enemy(int enemy_number);

#endif
