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

#define DECOR_RAND 23 			// valeur de base
#define DECOR_COUNTER_RAND 11 	// valeur ajoutée à chaque apparition de décor

#define FUEL_INTERVAL_DELTA 500
#define JERRYCAN_FUEL 550  			//utilisé dans collision.c, pick_up_jerrycan()
extern int decor_counter_rand;

void init_level();
void update_level();
void create_road();
void create_enemy();
void create_decor();
void create_civilian();

#endif
