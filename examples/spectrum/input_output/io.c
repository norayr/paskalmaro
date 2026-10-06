/* io.c - ZX Spectrum hardware I/O helpers.
 *
 * rdport reads a 16-bit I/O port (keyboard, beeper, etc.).
 * The ZX Spectrum keyboard is read via IN A,(C) with the port high byte
 * selecting the keyboard row.  A bit reads 0 when the key is pressed.
 *
 * band is needed because ptc maps Pascal 'and' to logical C '&&',
 * not bitwise '&'.  Use band() to test individual key bits.
 *
 * inp() is provided by z88dk.  For bare SDCC, replace with an __asm stub.
 *
 * Compile with z88dk:
 *   zcc +zx -o prog prog.c io.c -create-app                          */

#include <stdlib.h>

void poke(unsigned int addr, int val)
{
    *((volatile unsigned char *)addr) = (unsigned char)val;
}

int band(int a, int b) { return a & b; }

/* Read a 16-bit I/O port.  z88dk provides inp() for this purpose. */
int rdport(unsigned int port)
{
    return (int)inp((unsigned int)port);
}
