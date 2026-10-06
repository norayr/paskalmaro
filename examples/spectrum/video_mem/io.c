/* io.c - ZX Spectrum memory-mapped I/O helpers.
 *
 * poke/peek give Pascal access to absolute memory addresses.
 * band/bor are needed because ptc maps Pascal 'and'/'or' to C '&&'/'||'
 * (logical operators), not '&'/'|' (bitwise).  For attribute byte
 * construction and port-value masking, bitwise ops are required.
 *
 * Compile with z88dk:
 *   zcc +zx -o prog prog.c io.c -create-app                          */

void poke(unsigned int addr, int val)
{
    *((volatile unsigned char *)addr) = (unsigned char)val;
}

int peek(unsigned int addr)
{
    return (int)*((volatile unsigned char *)addr);
}

int band(int a, int b) { return a & b; }
int bor (int a, int b) { return a | b; }
