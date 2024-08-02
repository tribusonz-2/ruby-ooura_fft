#ifndef FFT_ABI_H
#define FFT_ABI_H

#if defined(__cplusplus)
extern "C" {
#endif

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

#include <ruby.h> // rb_raise(), rb_eThreadError
#include "abi/fftsg/fftsg.h"

#if defined(__cplusplus)
}
#endif

#endif /* FFT_ABI_H */
