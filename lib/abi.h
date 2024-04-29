#ifndef FFT_ABI_H
#define FFT_ABI_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <ruby.h>
#include "missing.h"

// Ooura's FFT Thread Use Setting
#if defined(HAVE_CREATETHREAD) && \
    defined(HAVE_WAITFORSINGLEOBJECT) && \
    defined(HAVE_CLOSEHANDLE)
# define USE_CDFT_WINTHREADS
# define USING_THREAD  "Windows"
#elif defined(HAVE_PTHREAD_CREATE) && defined(HAVE_PTHREAD_JOIN)
# define USE_CDFT_PTHREADS
# define USING_THREAD  "POSIX"
#else
# define USING_THREAD  NULL
#endif
#include "abi/fftsg/fftsg.h"

#ifdef     USE_GLOBAL_VARIABLE
# define   RUBY_EXT_EXTERN
# define   GLOBAL_VAL(v) = (v)
#else
# define   RUBY_EXT_EXTERN    extern
# define   GLOBAL_VAL(v)
#endif

RUBY_EXT_EXTERN VALUE rb_mFFT;

#define MAKE_SYM(str)  ID2SYM(rb_intern(str))

#if defined(__cplusplus)
}
#endif

#endif /* FFT_ABI_H */
