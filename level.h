#ifndef LEVEL_H
#define LEVEL_H


//#include 

extern long scroll_y; //mis à jour dans update_control
extern char road_change;

#define TURN_LEFT 0x01
#define TURN_RIGHT 0x02
#define NARROW_LEFT 0x04
#define NARROW_RIGHT 0x08
#define NARROW_CENTER 0x16
#define EXPAND_LEFT 0x32
#define EXPAND_RIGHT 0x64

void init_level();
void update_level();
void create_road();
void create_enemy();
void create_decor();
void create_civilian();

#endif
