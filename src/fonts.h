#ifndef FONTS_H
#define FONTS_H

#define INTEGER_WIDTH 8

void init_score();
void add_score(int amount);
void write_thing(int x,int y, int thing);
void test_score(); //si le score actuel > le score enregistré, alors enregistrer le nouveau score
void save_score();

#endif
