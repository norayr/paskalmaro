;
; rom.s - Oric Atmos ROM wrappers (ca65/cc65 assembly)
;
; $F77C: low-level screen character write — expects char in both A and X.
;        This is the routine $CCD9 calls internally after cursor checks.
;        Calling it directly bypasses the CTRL-O suppress flag ($2E bit 7)
;        which the cc65 Oric runtime may leave set.
;
; $CBF0: NEWLINE — itself calls $CCD9, same CTRL-O risk; we use $F77C
;        directly with CR ($0D) and LF ($0A) instead.
;
; cc65 fastcall: last (only) char argument is already in A on entry.
; TAX copies it to X so both registers carry the char for $F77C.
;

        .export _putch, _newline

; void putch(char c)
_putch:
        tax                     ; char in A (fastcall) -> copy to X
        jmp     $F77C           ; direct screen write, both A and X = char

; void newline(void)
_newline:
        lda     #$0D            ; CR
        tax
        jsr     $F77C
        lda     #$0A            ; LF
        tax
        jmp     $F77C
