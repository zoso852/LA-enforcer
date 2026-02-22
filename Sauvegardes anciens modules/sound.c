#include <dos.h>
#include <conio.h>

#include "sound.h"

#define OPL_PORT 0x388
#define MAX_VOICES 9   // OPL2 a 9 voix FM

typedef enum {
    SOUND_NONE,
    SOUND_COLLISION,
    SOUND_EXPLOSION,
    SOUND_GUNSHOT
} SoundType;

typedef struct {
    int voice;               // numéro de la voix (0..8)
    SoundType type;          // type de son joué
    unsigned char active;    // 1 = en cours, 0 = libre
    int frames_remaining;    // durée restante en frames
} SoundEvent;

SoundEvent sounds[MAX_VOICES];

/* Mapping des offsets d'opérateurs OPL2 */
int opl_slot_offset[18] = {
    0,1,2,   8,9,10,   16,17,18,
    /* second bank: not used on OPL2 */
};

void update_sounds() {
	int i;
    for(i=0;i<MAX_VOICES;i++) {
        if(sounds[i].active) {
            sounds[i].frames_remaining--;
            if(sounds[i].frames_remaining <= 0) {
                // key-off + reset
                opl_stop_voice(sounds[i].voice);
                sounds[i].active = 0;
            }
        }
    }
}

int get_free_voice() {

	int i;
    for(i=0;i<MAX_VOICES;i++)
        if(!sounds[i].active) return i;
    return -1;  // aucune libre
}

// Key-off + reset opérateurs d'une voix
void opl_stop_voice(int voice) {
    opl_write(0xB0 + voice, 0x00);
    // reset opérateurs (mod + carrier)
    opl_write(0x20 + voice, 0);
    opl_write(0x40 + voice, 0);
    opl_write(0x60 + voice, 0);
    opl_write(0x80 + voice, 0);
    opl_write(0x23 + voice, 0);
    opl_write(0x43 + voice, 0);
    opl_write(0x63 + voice, 0);
    opl_write(0x83 + voice, 0);
    
    opl_write(0xA0 + voice, 0x00);
    opl_write(0xB0 + voice, 0x00);
}

/* Délais courts pour laisser le temps au chip */
void opl_delay()
{
    int i;
    for(i=0; i<6; i++) {;}
}

void opl_write(unsigned char reg, unsigned char val)
{
    outp(OPL_PORT, reg);
    opl_delay();
    outp(OPL_PORT+1, val);
    opl_delay();
}

/* Reset complet du OPL2 */
void opl_init()
{
    int r, i, off;

    /* Reset timers */
    opl_write(0x04, 0x60);  /* Disable timers */
    opl_write(0x04, 0x80);  /* Reset IRQ flag */

    /* Clear all operator registers */
    for(r = 0; r <= 0xF5; r++)
        opl_write(r, 0);

    /* Activate waveforms (OPL2 standard) */
    opl_write(0x01, 0x20);
    
        for(i = 0; i < 18; i++)
        {
            off = opl_slot_offset[i];
            opl_write(0xE0 + off, 0x00); // waveform 0 = sinus 
        }
}

/* Joue une note simple sur la voix 0 */
void opl_beep()
{
    /* Operator 1 (modulator) */
    opl_write(0x20 + 0, 0x01);  /* Multiple = 1 */
    opl_write(0x40 + 0, 0x10);  /* Level */
    opl_write(0x60 + 0, 0xF0);  /* Attack */
    opl_write(0x80 + 0, 0xF0);  /* Release */

    /* Operator 2 (carrier) */
    opl_write(0x23 + 0, 0x01);
    opl_write(0x43 + 0, 0x00);
    opl_write(0x63 + 0, 0xF0);
    opl_write(0x83 + 0, 0xF0);

    /* Frequency : 440 Hz (approx) */
    opl_write(0xA0 + 0, 0x98);  /* low bits */
    opl_write(0xB0 + 0, 0x01 | 0x20);  /* octave 1 + key-on */
}

void opl_stop()
{
    int r;
    /* Key-off pour toutes les voix */
    for(r = 0; r < 9; r++)
        opl_write(0xB0 + r, 0x00);

    /* Clear tous les registres d’opérateurs */
    for(r = 0; r <= 0xF5; r++)
        opl_write(r, 0x00);

    /* Désactive timers et IRQ */
    opl_write(0x04, 0x60);  /* Disable timers */
    opl_write(0x04, 0x80);  /* Reset IRQ flag */
}

void play_collision()
{
    int voice = get_free_voice();
    if(voice<0) return; // aucune voix dispo

    sounds[voice].voice = voice;
    sounds[voice].type = SOUND_COLLISION;
    sounds[voice].active = 1;
    sounds[voice].frames_remaining = 3;  // dure ~50ms à 60FPS
    
    // Modulateur
    opl_write(0x20 + voice, 0x01);   // multiple
    opl_write(0x40 + voice, 0x00);   // volume max
    opl_write(0x60 + voice, 0xF0);   // attaque rapide
    opl_write(0x80 + voice, 0x10);   // decay court
    opl_write(0xE0 + voice, 0x00);   // waveform sinus
    
    // Carrier
    opl_write(0x23 + voice, 0x01);
    opl_write(0x43 + voice, 0x00);
    opl_write(0x63 + voice, 0xF0);
    opl_write(0x83 + voice, 0x10);
    opl_write(0xE3 + voice, 0x00);

    // Fréquence arbitraire pour impact
    opl_write(0xA0 + voice, 0x60);  // low bits Freq
    opl_write(0xB0 + voice, 0x01 | 0x20);  // octave + key-on

}

void play_test()
{
    int voice = get_free_voice();
    if(voice<0) return; // aucune voix dispo

    sounds[voice].voice = voice;
    sounds[voice].type = SOUND_COLLISION;
    sounds[voice].active = 1;
    sounds[voice].frames_remaining = 50;  // dure ~50ms à 60FPS
    
    // Modulateur
    opl_write(0x20 + voice, 0x01);		// 	7     6     5       4     3 2 1 0
									    //	Trem  Vib   Sustain KSR   Multiple
    opl_write(0x40 + voice, 0x0B);   	//	7 6 | 5 4 3 2 1 0
										//	KSL | volume (0–63), inversé 0 = max volume, 63 = silence
    
    opl_write(0x60 + voice, 0xF1);   	//	7 6 5 4 | 3 2 1 0
										//	Attack  | Decay   0 = lent
    
    opl_write(0x80 + voice, 0xAF);   	//	7 6 5 4 | 3 2 1 0
										//	Sustain | Release	0 = lent
										
    opl_write(0xE0 + voice, 0x02);   	// 0–1 = waveform :
										//	00 = sinus 01 = demi-sinus 10 = abs(sinus) 11 = pseudo-square
										
	 // Carrier
    opl_write(0x23 + voice, 0x11);		// 	7     6     5       4     3 2 1 0
									    //	Trem  Vib   Sustain KSR   Multiple
    opl_write(0x43 + voice, 0x00);   	//	7 6 | 5 4 3 2 1 0
										//	KSL | volume (0–63), inversé 0 = max volume, 63 = silence
    
    opl_write(0x63 + voice, 0xF1);   	//	7 6 5 4 | 3 2 1 0
										//	Attack  | Decay   0 = lent
    
    opl_write(0x83 + voice, 0x07);   	//	7 6 5 4 | 3 2 1 0
										//	Sustain | Release	0 = lent
										
    opl_write(0xE3 + voice, 0x02);   	// 0–1 = waveform :
										//	00 = sinus 01 = demi-sinus 10 = abs(sinus) 11 = pseudo-square
    
    // Fréquence arbitraire pour impact
    opl_write(0xA0 + voice, 0x2C); 			// low bits Freq
    
    opl_write(0xB0 + voice, 0x81 | 0x21);  	//7      6 5 4 | 3 2 1 0
											//KeyOn  Block | Freq high bits
										
	opl_write(0xC0 + voice, 0x01 | 0x00);	//7 6 5	          4 | 3 2 1 0
											//0 0 0=FM, 1=AM  0   0 Feedback 0-7 (7=METAL!!!)

    
}

void play_big_gun_shot()
{

}

void play_car_explosion()
{

}

