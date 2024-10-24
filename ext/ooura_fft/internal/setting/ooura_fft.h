#ifndef INTERNAL_SETTING_OOURAFFT_H
#define INTERNAL_SETTING_OOURAFFT_H

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

#endif /* INTERNAL_SETTING_OOURAFFT_H */
