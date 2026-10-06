/* Oric adapter, compiled by the target compiler to handle its own ABI.
 * Define PTC_OSDK for OSDK; otherwise compile for cc65's atmos target. */
#include "ptc_console.h"
#ifdef PTC_OSDK
#include <stdio.h>
extern int get(void); /* OSDK blocking input without echo; no arguments. */
#else
#include <conio.h>
#endif

void PtcPutChar(int character)
{
#ifdef PTC_OSDK
    /* OSDK putchar expands LF to CR/LF. */
    putchar((char)character);
#else
    if (character == '\n') cputc('\r');
    cputc((char)character);
#endif
}

int PtcGetChar(void)
{
    int character;
#ifdef PTC_OSDK
    character = get() & 127;
#else
    character = cgetc();
#endif
    /* This adapter echoes keys, but does not implement a line editor. */
    PtcPutChar(character == '\r' ? '\n' : character);
    return character;
}

void PtcFlush(void)
{
    /* The screen is unbuffered. */
}

void PtcConsoleFail(int code)
{
    const char *message = "\nConsole I/O error ";
    while (*message) PtcPutChar(*message++);
    PtcPutChar('0' + code);
    PtcPutChar('\n');
    for (;;) ;
}
