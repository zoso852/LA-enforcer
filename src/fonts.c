#include <stdio.h>
#include <time.h>
#include <string.h>

#include "display.h"
#include "fonts.h"
#include "control.h"

#define X_SCORE 261
#define Y_SCORE 165
#define SCORE_FILE "score.dat"



unsigned char far zero[42] = {7,10,2,6,7,254,1,8,7,254,0,2,7,8,2,7,254,0,1,7,4,2,7,9,1,7,254,0,2,7,8,2,7,254,1,8,7,254,2,6,7,255};
unsigned char far un[32] = {6,10,2,1,7,9,1,7,254,1,2,7,9,1,7,254,0,10,7,254,0,10,7,254,9,1,7,254,9,1,7,255};
unsigned char far deux[60] = {7,10,1,1,7,7,3,7,254,0,2,7,6,4,7,254,0,1,7,5,2,7,9,1,7,254,0,1,7,4,2,7,9,1,7,254,0,1,7,3,2,7,9,1,7,254,0,4,7,8,2,7,254,1,2,7,8,2,7,255};
unsigned char far trois[57] = {7,10,1,1,7,8,1,7,254,0,2,7,8,2,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,10,7,254,1,3,7,5,4,7,255};
unsigned char far quatre[42] = {7,10,4,2,7,254,3,3,7,254,2,2,7,5,1,7,254,1,2,7,5,1,7,9,1,7,254,0,10,7,254,0,10,7,254,5,1,7,9,1,7,255};
unsigned char far cinq[60] = {7,10,0,5,7,8,1,7,254,0,5,7,8,2,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,6,7,254,0,1,7,5,4,7,255};
unsigned char far six[48] = {7,10,2,7,7,254,1,9,7,254,0,2,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,4,6,7,254,5,4,7,255};
unsigned char far sept[39] = {7,10,0,2,7,254,0,2,7,254,0,1,7,6,4,7,254,0,1,7,5,5,7,254,0,1,7,4,2,7,254,0,5,7,254,0,4,7,255};
unsigned char far huit[54] = {7,10,1,3,7,5,4,7,254,0,10,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,10,7,254,1,3,7,5,4,7,255};
unsigned char far neuf[51] = {7,10,1,3,7,254,0,5,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,9,1,7,254,0,1,7,4,1,7,8,2,7,254,0,9,7,254,1,7,7,255};

int score;

static void write_score();

void add_score(signed int amount)
{
score+=amount;
if (score<0) score =0; 
write_score();
}

void init_score()
{

erase_score(X_SCORE,Y_SCORE);
score = 0;
write_score();
}

static void write_score()
{
int i;
char truc[7];

snprintf(truc, sizeof(truc), "%06d", score);


erase_score(X_SCORE,Y_SCORE);
for (i=0;i<6;i++)
	{
	switch (truc[i])
		{
		case '0' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,zero);
			break;
		case '1' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,un);
			break;
		case '2' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,deux);
			break;
		case '3' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,trois);
			break;
		case '4' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,quatre);
			break;
		case '5' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,cinq);
			break;
		case '6' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,six);
			break;
		case '7' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,sept);
			break;
		case '8' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,huit);
			break;
		case '9' :
			put_hud(X_SCORE+i*INTEGER_WIDTH,Y_SCORE<<4,neuf);			
			break;
		}
	}
}

void write_thing(int x, int y, int thing)
{
int i;
char truc[7];

snprintf(truc, sizeof(truc), "%06d", thing);


erase_score(x,y);
for (i=0;i<6;i++)
	{
	switch (truc[i])
		{
		case '0' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,zero);
			break;
		case '1' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,un);
			break;
		case '2' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,deux);
			break;
		case '3' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,trois);
			break;
		case '4' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,quatre);
			break;
		case '5' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,cinq);
			break;
		case '6' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,six);
			break;
		case '7' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,sept);
			break;
		case '8' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,huit);
			break;
		case '9' :
			put_sprite(x+i*INTEGER_WIDTH,y<<4,neuf);			
			break;
		}
	}
}

void xor_cipher(char *data, int len, char key)
{
	int i;
	
    for (i = 0; i < len; i++)
    {
        data[i] ^= key;
    }
}

void save_score()
{
    time_t now;
    struct tm *t;
    FILE *f;
    char buffer[256];
    int len;

    now = time(NULL);
    t = localtime(&now);

    // Construire le contenu en mémoire
    len = snprintf(buffer, sizeof(buffer),
        "la-enforcer-score-filev1.0\n%d\n%04d-%02d-%02d\nfalse",
        score,
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday
    );

    if (len <= 0)
        return;

    // Chiffrement XOR
    xor_cipher(buffer, len, 0x5A);

    f = fopen(SCORE_FILE, "wb"); // ⚠️ mode binaire
    if (f == NULL)
        return;

    fwrite(buffer, 1, len, f);
    fclose(f);
}

int load_score(void)
{
    FILE *f;
    char buffer[256];
    int len;
    int saved_score = 0;

    f = fopen(SCORE_FILE, "rb"); // mode binaire !
    if (f == NULL)
        return 0;

    len = fread(buffer, 1, sizeof(buffer)-1, f);
    fclose(f);

    if (len <= 0)
        return 0;

    // Déchiffrement XOR
    xor_cipher(buffer, len, 0x5A);

    buffer[len] = '\0'; // sécurité string

    // Parser
    sscanf(buffer, "la-enforcer-score-filev1.0\n%d", &saved_score);

    return saved_score;
}

int score_file_exists(void)
{
    FILE *f = fopen(SCORE_FILE, "rb");
    if (f != NULL)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

void test_score()
{
	if (score>load_score()) {
		save_score();
		display_highscore = 1;
		}
}


