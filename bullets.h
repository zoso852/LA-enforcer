#ifndef BULLETS_H
#define BULLETS_H

#include "entity.h"

#define MAX_BULLET 12

#define BULLET_ACTIVE 0x01

typedef struct {
    unsigned char far *sprite;
    short speed;
} BulletType;

#define REGULAR_BULLET 0

typedef struct {
    entity box;
    int type;
    unsigned char flags;
    int vx, vy;
} bullet_t;


extern BulletType bullet_types[1];
extern bullet_t bullet[];

extern void init_bullets();
extern void shoot_bullet(int enemy_number);
extern void update_bullets();

#endif
