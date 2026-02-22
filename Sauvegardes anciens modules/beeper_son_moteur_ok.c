#include <dos.h>
#include <i86.h>
#include <conio.h>

#include "beeper.h"

volatile int spk_ticks = 0;
volatile const unsigned short *snd_ptr = 0;
volatile int engine_freq = 0;              // fréquence du moteur en Hz
volatile unsigned short current_freq = 0;    // fréquence actuelle jouée


static const unsigned short snd_shoot[] = {80,150,200,180,150,110,85,60,0};
static const unsigned short snd_hit[] = {80,150,800,400,0};
static const unsigned short snd_collision[] = {60,100,80,55,0};
static const unsigned short snd_explosion[] = {120, 340, 200, 90, 400, 180, 60, 300,220, 100, 360, 140, 80, 260, 0};
static const unsigned short snd_bonus[] = {80,130,100,160,140,200,180,280,240,370,320,550,0};
static const unsigned short snd_squeal[] = {650,700,700,700,700,700,710,720,0};

void interrupt (*old_timer)(void);

void play_sound_id(sound_id_t id)
{
    switch (id)
    {
        case SND_SHOOT:
            play_sound(snd_shoot);
            break;
        case SND_HIT:
        	play_sound(snd_hit);
        	break;
        case SND_COLLISION:
            play_sound(snd_collision);
            break;
        case SND_EXPLOSION:
        	play_sound(snd_explosion);
        	break;
        case SND_BONUS:
        	play_sound(snd_bonus);
        	break;
        case SND_SQUEAL:
        	play_sound(snd_squeal);
        	break;
    }
}


void play_sound(const unsigned short *snd)
{
    snd_ptr = snd;
}

void __interrupt timer_isr(void)
{
    unsigned short f = 0;

    // priorité aux sons ponctuels
    if (snd_ptr)
    {
        f = *snd_ptr++;
        if (f == 0)
        {
            snd_ptr = 0;
            f = engine_freq; // reprend moteur si son ponctuel terminé
        }
    }
    else
    {
        f = engine_freq;  // moteur
    }

    if (f > 0)
    {
        if (f != current_freq)  // on change la fréquence uniquement si elle a changé
        {
            unsigned int div = 1193180 / f;
            outp(0x43, 0xB6);
            outp(0x42, div & 0xFF);
            outp(0x42, div >> 8);
            current_freq = f;
        }
        outp(0x61, inp(0x61) | 3);  // active le speaker
    }
    else
    {
        outp(0x61, inp(0x61) & ~3);  // coupe le speaker si rien à jouer
        current_freq = 0;
    }

    old_timer();
}

void init_sound(void)
{
    old_timer = _dos_getvect(0x08);
    _dos_setvect(0x08, timer_isr);
}

void sound_shutdown(void)
{
    _dos_setvect(0x08, old_timer);
}

void beep_async(int freq, int duration_ms)
{
    unsigned int div;

    div = 1193180 / freq;

    outp(0x43, 0xB6);
    outp(0x42, div & 0xFF);
    outp(0x42, div >> 8);

    outp(0x61, inp(0x61) | 3);

    /* 18.2 ticks/sec */
    spk_ticks = (duration_ms * 18) / 1000;
}
