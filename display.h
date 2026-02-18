#ifndef DISPLAY_H
#define DISPLAY_H

#define FUEL_X  260
#define FUEL_Y  125
#define FUEL_W  50
#define FUEL_H  6
#define FUEL_COLOR 10
#define EMPTY_COLOR 0
#define FUEL_BASE (FUEL_Y*320 + FUEL_X)

void init_display();
void set_palette(unsigned char *pal);
void set_palette_fade_out();
void set_palette_fade_in(unsigned char *pal);
void update_display();
void clear_buffer();
void flip();
void flip1 (unsigned short seg, unsigned short off);
void put_sprite(int x, signed int y, unsigned char far *sprite);
void init_hud();
void draw_speed_cursor(int cx);
void draw_full_fuel(void);
void update_fuel_gauge(int fuel_new);
void free_buffer();

#endif
