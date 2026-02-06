#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define SB_BASE 0x220

int sb_reset() {
    int t;

    outp(SB_BASE+0x6,1);
    delay(1);
    outp(SB_BASE+0x6,0);

    for(t=0;t<10000;t++) {
        if(inp(SB_BASE+0xE) & 0x80) {
            if(inp(SB_BASE+0xA) == 0xAA)
                return 1;
        }
    }
    return 0;
}

void sb_write(unsigned char v) {
    while(inp(SB_BASE+0xC) & 0x80);
    outp(SB_BASE+0xC, v);
}

unsigned char sb_read() {
    while(!(inp(SB_BASE+0xE) & 0x80));
    return inp(SB_BASE+0xA);
}

/* ----- Vrai playback à 8000 Hz via 0x10 ----- */

void sb_set_sample_rate(int hz) {
    int tc;
    tc = 256 - (1000000 / hz);
    sb_write(0x40);      /* Set Time Constant */
    sb_write(tc);
}

/* Joue un buffer entier avec timing DSP */
void sb_play_buffer(unsigned char *buf, int len) {
    int i;

    sb_set_sample_rate(8000);

    for(i = 0; i < len; i++) {
        sb_write(0x10);       /* play byte */
        sb_write(buf[i]);
    }
}

/* ----- Génération bruit de mitrailleuse ----- */

#define MG_LEN 400
unsigned char mg_buf[MG_LEN];

void gen_machinegun() {
    int i;
    for(i=0;i<MG_LEN;i++) {
        mg_buf[i] = 128 + ((rand()%80) - 40);
        if((i % 35) == 0) mg_buf[i] = 255;  /* impulsion */
    }
}

/* ----- Génération bruit de collision ----- */

#define COLL_LEN 500
unsigned char coll_buf[COLL_LEN];

void gen_collision() {
    int i;
    for(i=0;i<COLL_LEN;i++) {
        int noise = (rand()%70) - 35;
        int impact = (i < 50 ? (50 - i) : 0);
        coll_buf[i] = 128 + noise + impact;
    }
}

/* ----- Main ----- */

int main() {
    char ch;

    if(!sb_reset()) {
        printf("SB non detectee\n");
        return 0;
    }

    printf("DSP version : ");
    sb_write(0xE1);
    printf("%d.%d\n", sb_read(), sb_read());

    gen_machinegun();
    gen_collision();

    printf("m = mitrailleuse\n");
    printf("c = collision\n");
    printf("q = quitter\n");

    while(1) {
        if(kbhit()) {
            ch = getch();

            if(ch == 'q') break;
            if(ch == 'm') sb_play_buffer(mg_buf, MG_LEN);
            if(ch == 'c') sb_play_buffer(coll_buf, COLL_LEN);
        }
    }

    return 0;
}
