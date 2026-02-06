#include <dos.h>
#include <conio.h>

#define OPL_PORT 0x388

/* Mapping des offsets d'opérateurs OPL2 */
int opl_slot_offset[18] = {
    0,1,2,   8,9,10,   16,17,18,
    /* second bank: not used on OPL2 */
};




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


/* Programme de test */
int main()
{
    opl_init();

    //opl_beep();

    delay(400);
    
    play_collision();
    delay(500);

    opl_stop();

    return 0;
}

void play_collision()
{
    int voice = 0;  // utiliser la voix 0 (opérateurs 0 et 3)
    
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

    delay(50);

    // Key-off
    opl_write(0xB0 + voice, 0x00);
}

