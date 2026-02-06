#ifndef DISPLAY_H
#define DISPLAY_H

void init_display();
void update_display();
void clear_buffer();
void flip();
void flip1 (unsigned short seg, unsigned short off);
void put_sprite(int x, signed int y, unsigned char far *sprite);
void free_buffer();
void put_sprite_gpt(int x, signed int y, unsigned char far *sprite);


#endif
