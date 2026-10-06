(* video.p - ZX Spectrum video memory access via poke/peek.
 *
 * Pixel memory:     $4000 (16384), 6144 bytes.
 * Attribute memory: $5800 (22528),  768 bytes (one byte per 8x8 cell).
 *
 * Attribute byte layout: FLASH | BRIGHT | P2 P1 P0 | I2 I1 I0
 *   INK colours:  0=black 1=blue 2=red 3=magenta 4=green 5=cyan 6=yellow 7=white
 *   PAPER colours use the same encoding, shifted left by 3.
 *   BRIGHT = bit 6.
 *
 * Note: ptc maps Pascal 'and'/'or' to logical C '&&'/'||', not bitwise.
 * Use the band/bor external wrappers for bitwise attribute construction.
 *
 * Build:
 *   ptc -m -i16 < video.p > video.c
 *   zcc +zx -o video video.c io.c -create-app                        *)

program video_mem;

const
    pixbuf = 16384;   { $4000 - pixel memory:      6144 bytes }
    attbuf = 22528;   { $5800 - attribute memory:   768 bytes }

    bright = 64;      { bit 6 of attribute byte }
    white  = 7;       { INK 7 }
    cyan   = 5;       { INK 5 }

type address = 0..65535;

var
    i : integer;

procedure poke(addr : address; val : integer); external;
function  peek(addr : address)      : integer; external;
function  bor (a, b : integer)      : integer; external;

begin
    { Set all pixels to ink colour (all bits on) }
    for i := 0 to 6143 do
        poke(pixbuf + i, 255);

    { Alternate attribute cells: bright white / bright cyan }
    for i := 0 to 767 do
        if i mod 2 = 0 then
            poke(attbuf + i, bor(bright, white))   { $47 = 71 }
        else
            poke(attbuf + i, bor(bright, cyan))    { $45 = 69 }
end.
