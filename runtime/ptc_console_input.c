/* Lazy console input. No FILE, scanf, allocation, or system headers.
 * The supported host and 6502 targets have two's-complement native integers. */
#include "ptc_console.h"

#define NO_LOOKAHEAD (-3)
#define NATIVE_MAX ((unsigned int)(~0U) >> 1)

static int lookahead = NO_LOOKAHEAD;
static int skip_lf;

static int peek(void)
{
    int value;
    if (lookahead == NO_LOOKAHEAD) {
        value = PtcGetChar();
        if (skip_lf) {
            skip_lf = 0;
            if (value == '\n') value = PtcGetChar();
        }
        if (value < PTC_CONSOLE_EOF || value > 255)
            PtcConsoleFail(PTC_CONSOLE_DEVICE_ERROR);
        if (value == '\r') {
            value = '\n';
            /* Do not fetch another character now: a keyboard could block. */
            skip_lf = 1;
        }
        lookahead = value;
    }
    return lookahead;
}

static void consume(void)
{
    lookahead = NO_LOOKAHEAD;
}

static int whitespace(int value)
{
    return value == ' ' || (value >= '\t' && value <= '\r');
}

static void skip_space(void)
{
    while (whitespace(peek())) consume();
    if (peek() == PTC_CONSOLE_EOF) PtcConsoleFail(PTC_CONSOLE_END);
}

int PtcConsoleEof(void)
{
    return peek() == PTC_CONSOLE_EOF;
}

int PtcConsoleEoln(void)
{
    int value = peek();
    return value == '\n' || value == PTC_CONSOLE_EOF;
}

int PtcReadChar(void)
{
    int value = peek();
    if (value == PTC_CONSOLE_EOF) PtcConsoleFail(PTC_CONSOLE_END);
    consume();
    /* Pascal text reads represent a line boundary by a space. */
    return value == '\n' ? ' ' : value;
}

static unsigned int magnitude(unsigned int limit)
{
    unsigned int value = 0;
    int digit;
    if (peek() < '0' || peek() > '9') PtcConsoleFail(PTC_CONSOLE_BAD_INPUT);
    while (peek() >= '0' && peek() <= '9') {
        digit = peek() - '0';
        if (value > limit / 10 ||
            (value == limit / 10 && (unsigned int)digit > limit % 10))
            PtcConsoleFail(PTC_CONSOLE_RANGE);
        value = value * 10 + (unsigned int)digit;
        consume();
    }
    return value;
}

int PtcReadInt(void)
{
    unsigned int value;
    int negative;
    skip_space();
    negative = peek() == '-';
    if (peek() == '-' || peek() == '+') consume();
    value = magnitude(NATIVE_MAX + (unsigned int)negative);
    if (!negative) return (int)value;
    if (value == 0) return 0;
    /* Avoid converting the unsigned magnitude of INT_MIN to signed int. */
    return -(int)(value - 1) - 1;
}

int PtcReadSigned(int lower, int upper)
{
    int value = PtcReadInt();
    if (value < lower || value > upper) PtcConsoleFail(PTC_CONSOLE_RANGE);
    return value;
}

unsigned int PtcReadUInt(unsigned int lower, unsigned int upper)
{
    unsigned int value;
    skip_space();
    if (peek() == '+') consume();
    value = magnitude(upper);
    if (value < lower) PtcConsoleFail(PTC_CONSOLE_RANGE);
    return value;
}

void PtcReadWord(char *buffer, int length)
{
    int i = 0;
    if (length <= 0) PtcConsoleFail(PTC_CONSOLE_RANGE);
    skip_space();
    while (peek() != PTC_CONSOLE_EOF && !whitespace(peek())) {
        if (i == length) PtcConsoleFail(PTC_CONSOLE_WORD_TOO_LONG);
        buffer[i++] = (char)peek();
        consume();
    }
    while (i < length) buffer[i++] = ' ';
}

void PtcReadLn(void)
{
    while (peek() != '\n' && peek() != PTC_CONSOLE_EOF) consume();
    if (peek() == '\n') consume();
    /* No prefetch of the next line; EOF stays sticky. */
}
