#ifndef RUBY_EXT_EXTERN_H_INCLUDED
#define RUBY_EXT_EXTERN_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef     USE_GLOBAL_VARIABLE
# define   RUBY_EXT_EXTERN
#else
# define   RUBY_EXT_EXTERN    extern
#endif

#if defined(__cplusplus)
}
#endif

#endif /* RUBY_EXT_EXTERN_H_INCLUDED */
