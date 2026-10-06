/* Hosted adapter. System headers belong here, not in the Pascal helpers. */
#include "ptc_console.h"
#include <stdio.h>
#include <stdlib.h>

void PtcPutChar(int character)
{
    if (putchar(character) == EOF) PtcConsoleFail(PTC_CONSOLE_DEVICE_ERROR);
}

int PtcGetChar(void)
{
    int value = getchar();
    if (value == EOF && ferror(stdin)) return PTC_CONSOLE_IO_ERROR;
    return value;
}

void PtcFlush(void)
{
    if (fflush(stdout) == EOF) PtcConsoleFail(PTC_CONSOLE_DEVICE_ERROR);
}

void PtcConsoleFail(int code)
{
    const char *reason;
    switch (code) {
    case PTC_CONSOLE_END: reason = "unexpected end of input"; break;
    case PTC_CONSOLE_BAD_INPUT: reason = "invalid integer"; break;
    case PTC_CONSOLE_RANGE: reason = "value or field width out of range"; break;
    case PTC_CONSOLE_WORD_TOO_LONG: reason = "input word too long"; break;
    default: reason = "device error"; break;
    }
    fputs("Console I/O: ", stderr);
    fputs(reason, stderr);
    fputc('\n', stderr);
    exit(code);
}
