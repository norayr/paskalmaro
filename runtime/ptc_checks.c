/* Link once with sources translated using ptc -e.
 * PTC_MINIMAL: no libc; stop forever on failure.
 * PTC_CUSTOM_FAIL: provide your own non-returning PtcFail(int). */
#include "ptc_checks.h"

#ifndef PTC_CUSTOM_FAIL
#ifndef PTC_MINIMAL
#include <stdio.h>
#include <stdlib.h>
#endif

void PtcFail(int code)
{
#ifdef PTC_MINIMAL
    (void)code;
    for (;;) ;
#else
    fprintf(stderr, "Fatal: %s\n",
        code == 1 ? "nil pointer dereference" :
        code == 2 ? "array index out of bounds" : "missing case limb");
    exit(1);
#endif
}
#endif

void *Chknil(void *pointer)
{
    if (!pointer) PtcFail(1);
    return pointer;
}

int Chkidx(int index, int lower, int upper)
{
    if (index < lower || index > upper) PtcFail(2);
    return index;
}
