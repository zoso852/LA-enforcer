#ifndef EFFECTS_H
#define EFFECTS_H

#include "display.h"

#define MAX_EFFECTS 32

#define EFFECT_EXPLOSION 0
#define EFFECT_IMPACT    1
#define EFFECT_SMOKE     2

typedef struct {
    int x, y;
    char type;          // EFFECT_EXPLOSION, EFFECT_IMPACT, EFFECT_SMOKE...
    int frame;
    int timer;
    int active;
} effect_t;

extern effect_t effects[MAX_EFFECTS];

void init_effects();
void spawn_effect(int x, int y, int type);
void update_effects();
void draw_effects();

#endif
