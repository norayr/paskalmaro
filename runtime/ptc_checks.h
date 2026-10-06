/* Optional PTC checks. No C library or target headers are required here. */
#ifndef PTC_CHECKS_H
#define PTC_CHECKS_H

/* Failure codes: 1 = nil, 2 = array bounds, 3 = missing case arm.
 * A custom PtcFail MUST NOT return. */
void PtcFail(int code);
void *Chknil(void *pointer);
int Chkidx(int index, int lower, int upper);

#endif
