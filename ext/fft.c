/*******************************************************************************
	fft.c -- FFT
*******************************************************************************/
#include <ruby.h>
#include "ruby/fft/globals.h"
#include "abi.h"
#include "missing.h"

static void InitVM_FFTMain(void);
void
InitVM_FFT(void)
{
	InitVM(FFTMain);
}

int
opts_inversion_p(VALUE opts)
{
    static ID kwds[1];
    VALUE inversion;
    if (!kwds[0]) {
        kwds[0] = rb_intern_const("inversion");
    }
    rb_get_kwargs(opts, kwds, 0, 1, &inversion);
    switch (inversion) {
      case Qtrue: case Qfalse:
        break;
      case Qundef:
        return 0;
      default:
        rb_raise(rb_eArgError, "true or false is expected as inversion: %+"PRIsVALUE,
                 inversion);
    }

    return inversion == Qtrue;
}

static VALUE
fft_callback(int argc, VALUE *argv, VALUE (*callback_func)(VALUE, int))
{
	VALUE ary, opts = Qnil;
	rb_scan_args(argc, argv, "11", &ary, &opts);
	int invertible = opts_inversion_p(opts) ? -1 : 1;

	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eTypeError, "not an Array");

	return callback_func(ary, invertible);
}


#include "internal/solver/fft/cdft.h" // fft_cdft_inline()
/*
 *  call-seq:
 *    FFT.cdft(ary) -> [*Complex]
 *    FFT.cdft(ary, inversion: true) -> [*Complex]
 *  
 *  Treats the argument ary as a sequence of complex numbers, and performs the FFT.
 *  
 *  Function type          : Complex Discrete Fourier Transform
 *  Array size requirement : n >= 1, n = power of 2
 *  
 *  @ary       ... Specifies the array Class. All elements are assumed to be of class Complex.
 *                 If there is an element that is not the case, an implicit type conversion is performed.
 *  @inversion ... Keyword argument. Boolean. Defaults to false.
 *                 Setting it to true will switch to inverse FFT.
 *  retval     ... Sequence of the complex numbers
 */
static VALUE
fft_cdft(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_cdft_inline);
}


#include "internal/solver/fft/rdft.h" // fft_rdft_inline()
/*
 *  call-seq:
 *    FFT.rdft(ary) -> [*Float]
 *    FFT.rdft(ary, inversion: true) -> [*Float]
 *  
 *  Treats the argument ary as a sequence of real numbers, and performs the FFT.
 *  
 *  Function type          : Real Discrete Fourier Transform
 *  Array size requirement : n >= 2, n = power of 2
 *  
 *  @ary       ... Specifies the array Class. All elements are assumed to be of class Float.
 *                 If there is an element that is not the case, an implicit type conversion is performed.
 *  @inversion ... Keyword argument. Boolean. Defaults to false.
 *                 Setting it to true will switch to inverse FFT.
 *  retval     ... Sequence of the real numbers
 */
static VALUE
fft_rdft(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_rdft_inline);
}


#include "internal/solver/fft/ddct.h" // fft_ddct_inline()
/*
 *  call-seq:
 *    FFT.ddct(ary) -> [*Float]
 *    FFT.ddct(ary, inversion: true) -> [*Float]
 *  
 *  Treats the argument ary as a sequence of real numbers, and performs the FFT.
 *  
 *  Function type          : Discrete Cosine Transform
 *  Array size requirement : n >= 2, n = power of 2
 *  
 *  @ary       ... Specifies the array Class. All elements are assumed to be of class Float.
 *                 If there is an element that is not the case, an implicit type conversion is performed.
 *  @inversion ... Keyword argument. Boolean. Defaults to false.
 *                 Setting it to true will switch to inverse FFT.
 *  retval     ... Sequence of the real numbers
 */
static VALUE
fft_ddct(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_ddct_inline);
}


#include "internal/solver/fft/ddst.h" // fft_ddst_inline()
/*
 *  call-seq:
 *    FFT.ddst(ary) -> [*Float]
 *    FFT.ddst(ary, inversion: true) -> [*Float]
 *  
 *  Treats the argument ary as a sequence of real numbers, and performs the FFT.
 *  
 *  Function type          : Discrete Sine Transform
 *  Array size requirement : n >= 2, n = power of 2
 *  
 *  @ary       ... Specifies the array Class. All elements are assumed to be of class Float.
 *                 If there is an element that is not the case, an implicit type conversion is performed.
 *  @inversion ... Keyword argument. Boolean. Defaults to false.
 *                 Setting it to true will switch to inverse FFT.
 *  retval     ... Sequence of the real numbers
 */
static VALUE
fft_ddst(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_ddst_inline);
}


#include "internal/solver/fft/dfct.h" // fft_dfct_inline()
/*
 *  call-seq:
 *    FFT.dfct(ary) -> [*Float]
 *    FFT.dfct(ary, inversion: true) -> [*Float]
 *  
 *  Treats the argument ary as a sequence of real numbers, and performs the FFT.
 *  
 *  Function type          : Cosine Transform of RDFT (Real Symmetric DFT)
 *  Array size requirement : n >= 2, n = power of 2
 *  
 *  @ary       ... Specifies the array Class. All elements are assumed to be of class Float.
 *                 If there is an element that is not the case, an implicit type conversion is performed.
 *  @inversion ... Keyword argument. Boolean. Defaults to false.
 *                 Setting it to true will switch to inverse FFT.
 *  retval     ... Sequence of the real numbers
 */
static VALUE
fft_dfct(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_dfct_inline);
}


#include "internal/solver/fft/dfst.h" // fft_dfst_inline()
/*
 *  call-seq:
 *    FFT.dfst(ary) -> [*Float]
 *    FFT.dfst(ary, inversion: true) -> [*Float]
 *  
 *  Treats the argument ary as a sequence of real numbers, and performs the FFT.
 *  
 *  Function type: Sine Transform of RDFT (Real Anti-symmetric DFT)
 *  Array size requirement : n >= 2, n = power of 2
 *  
 *  @ary       ... Specifies the array Class. All elements are assumed to be of class Float.
 *                 If there is an element that is not the case, an implicit type conversion is performed.
 *  @inversion ... Keyword argument. Boolean. Defaults to false.
 *                 Setting it to true will switch to inverse FFT.
 *  retval     ... Sequence of the real numbers
 */
static VALUE
fft_dfst(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_dfst_inline);

}

static void
InitVM_FFTMain(void)
{
	rb_define_module_function(rb_mFFT, "cdft", fft_cdft, -1);
	rb_define_module_function(rb_mFFT, "rdft", fft_rdft, -1);
	rb_define_module_function(rb_mFFT, "ddct", fft_ddct, -1);
	rb_define_module_function(rb_mFFT, "ddst", fft_ddst, -1);
	rb_define_module_function(rb_mFFT, "dfct", fft_dfct, -1);
	rb_define_module_function(rb_mFFT, "dfst", fft_dfst, -1);
	rb_define_const(rb_mFFT, "USING_THREAD", rb_str_new_cstr((const char *)USING_THREAD));
}
