/* Pascal console formatting, without printf, allocation, or system headers. */
#include "ptc_console.h"

static void pad(int width, int length)
{
    if (width < 0) PtcConsoleFail(PTC_CONSOLE_RANGE);
    while (width > length) {
        PtcPutChar(' ');
        width--;
    }
}

void PtcWriteChar(int character, int width)
{
    pad(width, 1);
    PtcPutChar((unsigned char)character);
}

void PtcWriteText(const char *text, int length, int width)
{
    int i;
    if (length == -1) {
        length = 0;
        while (text[length]) length++;
    }
    if (length < 0) PtcConsoleFail(PTC_CONSOLE_RANGE);
    pad(width, length);
    for (i = 0; i < length; i++) PtcPutChar((unsigned char)text[i]);
}

static void number(unsigned int value, int negative, int width)
{
    /* Three decimal digits per byte suffice for the native unsigned type. */
    char digits[sizeof(unsigned int) * 3 + 1];
    int length = 0;
    do {
        digits[length++] = (char)('0' + value % 10);
        value /= 10;
    } while (value);
    pad(width, length + negative);
    if (negative) PtcPutChar('-');
    while (length) PtcPutChar(digits[--length]);
}

void PtcWriteInt(int value, int width)
{
    /* Unsigned negation also handles the most negative signed int. */
    unsigned int magnitude = (unsigned int)value;
    if (value < 0) magnitude = 0U - magnitude;
    number(magnitude, value < 0, width);
}

void PtcWriteUInt(unsigned int value, int width)
{
    number(value, 0, width);
}

void PtcWriteBool(int value, int width)
{
    PtcWriteText(value ? "true" : "false", value ? 4 : 5, width);
}
