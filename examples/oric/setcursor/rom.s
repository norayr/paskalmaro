;
; rom.s - Oric Atmos ROM wrappers (ca65/cc65 assembly)
;
; All character output goes to $F77C directly, bypassing $CCD9 and
; its CTRL-O suppress check ($2E bit 7).
; $F77C expects char in both A and X.
;
; setcursor(col, row): positions the cursor by writing to the Oric OS
; cursor locations and calling $DA0C to recalculate the screen address.
;   $0268: cursor row    (0..27)
;   $0269: cursor column (0..39)
;   $DA0C: recalculate screen row address
;
; cc65 fastcall two-int call: row (last param) in A/X; col on software stack.
;

        .importzp sp
        .export _putch, _newline, _clrscr, _setcursor

_putch:
        tax
        jmp     $F77C

_newline:
        lda     #$0D
        tax
        jsr     $F77C
        lda     #$0A
        tax
        jmp     $F77C

_clrscr:
        lda     #$0C
        tax
        jmp     $F77C

; void setcursor(int col, int row)
_setcursor:
        sta     $0268           ; row: low byte in A (fastcall last param)
        ldy     #0
        lda     (sp),y          ; col: low byte from cc65 software stack
        sta     $0269
        lda     sp              ; pop 2 bytes (one int) from software stack
        clc
        adc     #2
        sta     sp
        bcc     :+
        inc     sp+1
:       jmp     $DA0C           ; recalculate screen row address
