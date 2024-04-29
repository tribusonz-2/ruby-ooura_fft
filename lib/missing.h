#ifndef FFT_MISSING_H
#define FFT_MISSING_H

#if defined(__cplusplus)
extern "C" {
#endif

// C2X newer library
#ifdef HAVE_STDBIT_H
# include <stdbit.h>
#endif

#ifndef HAVE_ISPOW2L
extern bool ispow2l(long);
# include "missing/ispow2l.c"
#endif

#if defined(__cplusplus)
}
#endif

#endif /* FFT_MISSING_H */
