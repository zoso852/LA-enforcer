#include <dos.h>

int main(void)
{
    unsigned int div;
    unsigned char tmp;

    /* 440 Hz */
    div = 1193180 / 440;

    /* PIT canal 2, mode 3 */
    outp(0x43, 0xB6);
    outp(0x42, div & 0xFF);
    outp(0x42, div >> 8);

    /* Speaker ON */
    tmp = inp(0x61);
    outp(0x61, tmp | 3);

    /* Attendre ~1 seconde */
    delay(1000);

    /* Speaker OFF */
    tmp = inp(0x61);
    outp(0x61, tmp & ~3);

    return 0;
}
