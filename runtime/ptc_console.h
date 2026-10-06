/* Optional console backend. This interface needs no system headers.
 * Compile all translations, helpers, and the adapter with the same C ABI. */
#ifndef PTC_CONSOLE_H
#define PTC_CONSOLE_H

#define PTC_CONSOLE_EOF (-1)
#define PTC_CONSOLE_IO_ERROR (-2)

#define PTC_CONSOLE_END 1
#define PTC_CONSOLE_BAD_INPUT 2
#define PTC_CONSOLE_RANGE 3
#define PTC_CONSOLE_DEVICE_ERROR 4
#define PTC_CONSOLE_WORD_TOO_LONG 5

/* Implement these four functions in exactly one device adapter.
 * GetChar returns 0..255, EOF, or IO_ERROR. PutChar receives a byte, with
 * LF as the logical newline. Fail MUST NOT return. No initialization needed. */
void PtcPutChar(int character);
int PtcGetChar(void);
void PtcFlush(void);
void PtcConsoleFail(int code);

/* Output helpers; link ptc_console_output.c when output is used.
 * Width is a nonnegative minimum width. Text length -1 means NUL-terminated;
 * a nonnegative length writes that many bytes, including embedded NULs. */
void PtcWriteChar(int character, int width);
void PtcWriteText(const char *text, int length, int width);
void PtcWriteInt(int value, int width);
void PtcWriteUInt(unsigned int value, int width);
void PtcWriteBool(int value, int width);

/* Input helpers; link ptc_console_input.c when input is used.
 * One shared input state, including across separately translated Pascal files.
 * CRLF and bare CR normalize to LF. Eof/Eoln peek without consuming input. */
int PtcConsoleEof(void);
int PtcConsoleEoln(void);
int PtcReadChar(void);
int PtcReadInt(void);
int PtcReadSigned(int lower, int upper);
unsigned int PtcReadUInt(unsigned int lower, unsigned int upper);
void PtcReadWord(char *buffer, int length);
void PtcReadLn(void);

#endif
