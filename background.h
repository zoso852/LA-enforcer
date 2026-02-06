#ifndef BACKGROUND_H
#define BACKGROUND_H


extern unsigned char roadbuf[200][250];
extern unsigned short left_road, right_road;
extern unsigned short L_road[200], R_road[200];
extern int scroll_speed;
extern signed int top_line;

void init_background();
void gen_background(unsigned char far *buf);
void update_background();

#endif
