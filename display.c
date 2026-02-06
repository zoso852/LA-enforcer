#include <conio.h>
#include <i86.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>

#include "display.h"
#include "background.h"

unsigned char far *buffer;

unsigned short buffer_seg;
unsigned short buffer_off;


void init_display () {

	/* Basculer en mode 13h */
    union REGS r;
    r.h.ah = 0x00;
    r.h.al = 0x13;
    int86(0x10, &r, &r);
    
	//init_buffer();
    buffer = _fmalloc(64000); 
    buffer_seg = FP_SEG(buffer);
    buffer_off = FP_OFF(buffer);
    
    _fmemset(buffer, 0x00,64000);
}


        
void update_display(){
int y, inc=0;
signed int line;
	
	line = top_line;
	
	for (y = 0; y < 200; y++)
    {
	_fmemcpy(buffer+inc, roadbuf[line], 250);
        

        inc += 320;

        line++;
        if (line == 200)
            line = 0;
	}
}

void clear_buffer()
{
    //_fmemset(buffer, 0x00, 64000);
}

void flip (){
	flip1(buffer_seg, buffer_off);
}

extern void flip1 (unsigned short seg, unsigned short off);
#pragma aux flip1 = \
    "push ds"         \
    "mov ds, ax"      \
    "mov si, dx"\
    "mov bx,0A000h"   \
    "mov es, bx"      \
    "xor di, di"      \
    "mov cx, 64000"   \
    "cld" \
    "rep movsb"       \
    "pop ds" \
    parm [ax] [dx] \
    modify [cx si di es];

void put_sprite(int x, signed int y, unsigned char far *sprite) {
	
	
	int last_y_sprite=0,inc=0, y_sprite, i=2, row, len, start;
	unsigned int offset;
	
	unsigned char color;
	
	unsigned char far *bufptr;
	
	y_sprite=sprite[i];
	y=y>>4;
	
	while (y_sprite!=255){
		
		if(x>249 || x<0) break;
		
		if (y_sprite==254) {
			x++;
			i++;
			y_sprite=sprite[i];
			continue;
			}
			
		i++;
		len = sprite[i];
		i++;
		start = y+y_sprite;
		
		//offset=start*320+x;
		offset = (start<<8)+(start<<6)+x;
		
		bufptr = buffer + offset;
		
		for (row=0;row<len;row++){
		
			color = sprite[i];
			if ((unsigned)(start + row) < 200) *bufptr = color;
			bufptr+=320;
			}
			
		i++;
		y_sprite=sprite[i];
	}

}


void free_buffer(){
	
    _ffree(buffer);
}

