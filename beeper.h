#ifndef BEEPER_H
#define BEEPER_H

typedef enum {
    SND_SHOOT,
    SND_HIT,
    SND_COLLISION,
    SND_EXPLOSION
} sound_id_t;

void interrupt timer_isr(void);

void play_sound(const unsigned short *snd);

void play_sound_id(sound_id_t id);

void init_sound(void);

void sound_shutdown(void);


#endif
