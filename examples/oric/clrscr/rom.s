;
; rom.s - Oric Atmos ROM wrappers (ca65/cc65 assembly)
;
; All calls go to $F77C directly, bypassing $CCD9 and its CTRL-O
; suppress check ($2E bit 7) which the cc65 Oric runtime may leave set.
;
; $F77C expects the character in both A and X.
; cc65 fastcall: last char argument arrives in A; TAX copies it to X.
;

        .export _putch, _newline, _clrscr

; void putch(char c)
_putch:
        tax
        jmp     $F77C

; void newline(void)
_newline:
        lda     #$0D
        tax
        jsr     $F77C
        lda     #$0A
        tax
        jmp     $F77C

; void clrscr(void)  --  Ctrl-L ($0C) clears the screen
_clrscr:
        lda     #$0C
        tax
        jmp     $F77C
