#ifndef RB_OOURAFFT_API_H_INCLUDED
#define RB_OOURAFFT_API_H_INCLUDED

#include <ruby/internal/value.h> // VALUE

VALUE rb_oourafft_cdft(VALUE, int);
VALUE rb_oourafft_rdft(VALUE, int);
VALUE rb_oourafft_ddct(VALUE, int);
VALUE rb_oourafft_ddst(VALUE, int);
VALUE rb_oourafft_dfct(VALUE, int);
VALUE rb_oourafft_dfst(VALUE, int);

#endif /* RB_OOURAFFT_API_H_INCLUDED */
