/* Pascal calls ordinary C functions. This compiler handles its own ABI.
 * OSDK's no-argument get() and cc65's cgetc() both block without echo. */
#ifdef PTC_OSDK
extern int get(void);
#else
#include <conio.h>
#endif

void oricpoke(unsigned int addr, int value)
{
    *((volatile unsigned char *)addr) = (unsigned char)value;
}

int orickey(void)
{
#ifdef PTC_OSDK
    return get() & 127;
#else
    return cgetc();
#endif
}
