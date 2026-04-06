#include <stdio.h>
#include <time.h>

#include "display.h"
#include "fonts.h"

#define X_SCORE 261
#define Y_SCORE 165



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
char truc[4];

snprintf(truc, sizeof(truc), "%03d", thing);


erase_score(x,y);
for (i=0;i<3;i++)
	{
	switch (truc[i])
		{
		case '0' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,zero);
			break;
		case '1' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,un);
			break;
		case '2' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,deux);
			break;
		case '3' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,trois);
			break;
		case '4' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,quatre);
			break;
		case '5' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,cinq);
			break;
		case '6' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,six);
			break;
		case '7' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,sept);
			break;
		case '8' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,huit);
			break;
		case '9' :
			put_hud(x+i*INTEGER_WIDTH,y<<4,neuf);			
			break;
		}
	}
}

void save_score()
{
    time_t now;
    struct tm *t;
    FILE *f;

    now = time(NULL);
    t = localtime(&now);

    f = fopen("score.dat", "w");
    if (f == NULL)
        return;
        
	fprintf(f, "la-enforcer-score-filev1.0\n");
    fprintf(f, "%d\n", score);
    fprintf(f, "%04d-%02d-%02d\n",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday);
    fprintf(f, "false");

    fclose(f);
}

int load_score(void)
{
    FILE *f;
    int saved_score;
    char line[30];

    f = fopen("SCORE.DAT", "r");
    if (f == NULL)
        return 0;
        
    if (fgets(line, sizeof(line), f) == NULL)
    {
        fclose(f);
        return 0;
    }

    if (fscanf(f, "%d", &saved_score) != 1)
    {
        fclose(f);
        return 0;
    }

    fclose(f);
    
    return saved_score;
}

void test_score()
{
	if (score>load_score()) save_score();
}


