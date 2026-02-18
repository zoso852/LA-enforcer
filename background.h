#ifndef BACKGROUND_H
#define BACKGROUND_H


extern unsigned char roadbuf[200][250];
extern unsigned short left_road, right_road;
extern unsigned short L_road[200], R_road[200];
extern int scroll_speed;
extern signed int top_line;
extern unsigned char road_color;
extern unsigned int y_line_counter;

#define GRASS_COLOR 0xF6
#define ROAD_COLOR 0xF9
#define LINE_COLOR 0xFC

void init_background();
void create_tire_mark();
void gen_background(unsigned char far *buf);
void update_background();

#endif
