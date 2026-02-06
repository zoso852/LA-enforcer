#ifndef DECOR_H
#define DECOR_H


#include "entity.h"

#define MAX_DECOR 8

#define DECOR_ACTIVE 0x01  // 0000 0001
#define DECOR_SOLID  0x02  // 0000 0010

typedef struct {
    unsigned char far *sprite;
} DecorType;

#define PALM_TREE 0

typedef struct {
    entity box;
    int type;
    unsigned char flags;
} decor_t;

extern DecorType decor_types[1];

extern decor_t decor[];
extern int nb_decor;

void gen_decor(int x, int y, int type);

void update_decor();
void init_decor();

#endif
