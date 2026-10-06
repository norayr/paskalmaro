(* input.p - ZX Spectrum keyboard reading via hardware port I/O.
 *
 * The keyboard is read with IN A,(C) where the port high byte selects a
 * half-row of five keys.  A bit reads 0 when the key is pressed.
 *
 * Port addresses (16-bit):
 *   63486  $F7FE  keys: 1 2 3 4 5   (bits 0..4 = keys 1..5)
 *   61438  $EFFE  keys: 0 9 8 7 6   (bits 0..4 = keys 0,9,8,7,6)
 *   32766  $7FFE  keys: SPACE SYM M N B
 *
 * Arrow keys on a 48K Spectrum are CAPS SHIFT + 5/6/7/8:
 *   LEFT  = key 5  (port 63486, bit 4)
 *   DOWN  = key 6  (port 61438, bit 4)
 *   UP    = key 7  (port 61438, bit 3)
 *   RIGHT = key 8  (port 61438, bit 2)
 *
 * Note: ptc maps Pascal 'and' to logical C '&&', not bitwise '&'.
 * Use the band() external wrapper for port-bit testing.
 *
 * Build:
 *   ptc -m -i16 < input.p > input.c
 *   zcc +zx -o input input.c io.c -create-app                        *)

program keyboard_demo;

const
    attbuf     = 22528;   { $5800 - attribute memory }
    krow_12345 = 63486;   { $F7FE: keys 1..5 }
    krow_09876 = 61438;   { $EFFE: keys 0,9,8,7,6 }
    krow_space = 32766;   { $7FFE: SPACE,SYM,M,N,B }

    { Attribute byte values: BRIGHT + INK colour }
    attr_white = 71;   { bright white: BRIGHT + INK 7 }
    attr_green = 68;   { bright green: BRIGHT + INK 4 }
    attr_red   = 66;   { bright red:   BRIGHT + INK 2 }
    attr_cyan  = 69;   { bright cyan:  BRIGHT + INK 5 }

type address = 0..65535;

var
    k, row : integer;

procedure poke(addr : address; val : integer); external;
function  rdport(port : address)    : integer; external;
function  band(a, b : integer)      : integer; external;

begin
    { Make the top-left character cell solid INK so colour changes are visible.
      Its eight bitmap rows are 256 bytes apart in Spectrum screen memory. }
    for row := 0 to 7 do poke(16384 + row * 256, 255);
    { Set initial neutral attribute on top-left cell }
    poke(attbuf, attr_white);

    { Poll the keyboard and change the top-left attribute cell colour.
      5 = LEFT (green), 8 = RIGHT (cyan), SPACE = red, else white.
      'until false' loops forever (condition is never true).          }
    repeat
        k := rdport(krow_12345);
        if band(k, 16) = 0 then        { bit 4 = key 5, 0 when pressed }
            poke(attbuf, attr_green)
        else
            begin
                k := rdport(krow_09876);
                if band(k, 4) = 0 then { bit 2 = key 8, 0 when pressed }
                    poke(attbuf, attr_cyan)
                else
                    begin
                        k := rdport(krow_space);
                        if band(k, 1) = 0 then  { bit 0 = SPACE }
                            poke(attbuf, attr_red)
                        else
                            poke(attbuf, attr_white)
                    end
            end
    until false
end.
