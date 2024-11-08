/*******************************************************************************
	fft.c -- FFT
	
	$author$
*******************************************************************************/
#include <ruby.h>
#include "ooura_fft/globals.h"
#include "ooura_fft/api.h"
#include "internal/setting/ooura_fft.h"
#include "missing/missing.h"
#include "abi/fftsg/fftsg.h"


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

	Check_Type(ary, T_ARRAY);

	return callback_func(ary, invertible);
}

static inline VALUE
api_inline(VALUE ary, int inversion, VALUE (*callback_func)(VALUE, int))
{
	Check_Type(ary, T_ARRAY);
	return callback_func(ary, inversion);
}

/******************************************************************************/

#include "internal/solver/ooura_fft/cdft.h" // fft_cdft_inline()
/*
 * 
 * Document-method: cdft
 * 
 * call-seq:
 *   OouraFFT.cdft(ary)                  -> [*Complex]
 *   OouraFFT.cdft(ary, inversion: true) -> [*Complex]
 * 
 * Perform FFT with the first argument +ary+ as a numerical sequence.<br>
 * If the keyword argument +inversion+ is +true+, perform an inverse FFT.
 * 
 * [Function type]          Complex Discrete Fourier Transform
 * [Definition]
 *   [case1]  <tt>X[k] = sum_j=0^n-1 x[j]*exp(2*pi*i*j*k/n), 0<=k<n</tt>
 *   [case2]  <tt>X[k] = sum_j=0^n-1 x[j]*exp(-2*pi*i*j*k/n), 0<=k<n</tt>
 *   notes:: <tt>sum_j=0^n-1 is a summation from j=0 to n-1</tt>
 * [+ary+ sequence type]    All +Complex+
 * [Array size requirement] n >= 1, n = power of 2
 * 
 *   def sinewave(amp, f0, fs, n)
 *     amp * Math.sin(2 * Math::PI * f0 * n / fs)
 *   end
 *   
 *   OouraFFT.cdft(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
 *   #=> [(1.1441462984511075+0.0i),
 *   #=>  (-0.21410561232180816-0.3229641637954819i),
 *   #=>  (-0.14986404592245728-0.12744889477603982i),
 *   #=>  (-0.13944777827146557-0.05236611372238342i),
 *   #=>  (-0.13731142541964547+0.0i),
 *   #=>  (-0.13944777827146557+0.05236611372238342i),
 *   #=>  (-0.14986404592245728+0.12744889477603982i),
 *   #=>  (-0.21410561232180816+0.3229641637954819i)]
 */
static VALUE
fft_cdft(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_cdft_inline);
}

VALUE
rb_oourafft_cdft(VALUE ary, int inversion)
{
	return api_inline(ary, inversion, fft_cdft_inline);
}

/******************************************************************************/

#include "internal/solver/ooura_fft/rdft.h" // fft_rdft_inline()
/*
 * Document-method: rdft
 * 
 * call-seq:
 *   OouraFFT.rdft(ary)                  -> [*Float]
 *   OouraFFT.rdft(ary, inversion: true) -> [*Float]
 * 
 * Perform FFT with the first argument +ary+ as a numerical sequence.<br>
 * If the keyword argument +inversion+ is +true+, perform an inverse FFT.
 * 
 * [Function type]            Real Discrete Fourier Transform
 * [Definition]
 *   case1::  RDFT
 *     :: <tt>R[k] = sum_j=0^n-1 a[j]*cos(2*pi*j*k/n), 0<=k<=n/2</tt>
 *     :: <tt>I[k] = sum_j=0^n-1 a[j]*sin(2*pi*j*k/n), 0<k<n/2</tt>
 *   case2::  IRDFT (excluding scale)
 *     :: <tt>a[k] = (R[0] + R[n/2]*cos(pi*k))/2 + sum_j=1^n/2-1 R[j]*cos(2*pi*j*k/n) + sum_j=1^n/2-1 I[j]*sin(2*pi*j*k/n), 0<=k<n</tt>
 * [+ary+ sequence type]      All +Float+
 * [Array size requirement]   n >= 2, n = power of 2
 * 
 *   def sinewave(amp, f0, fs, n)
 *     amp * Math.sin(2 * Math::PI * f0 * n / fs)
 *   end
 *   
 *   OouraFFT.rdft(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
 *   # => [1.1441462984511075,
 *   # =>  -0.13731142541964547,
 *   # =>  -0.21410561232180816,
 *   # =>  -0.3229641637954819,
 *   # =>  -0.14986404592245728,
 *   # =>  -0.12744889477603982,
 *   # =>  -0.13944777827146557,
 *   # =>  -0.052366113722383416]
 */
static VALUE
fft_rdft(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_rdft_inline);
}

VALUE
rb_oourafft_rdft(VALUE ary, int inversion)
{
	return api_inline(ary, inversion, fft_rdft_inline);
}

/******************************************************************************/

#include "internal/solver/ooura_fft/ddct.h" // fft_ddct_inline()
/*
 * Document-method: ddct
 * 
 * call-seq:
 *   OouraFFT.ddct(ary)                  -> [*Float]
 *   OouraFFT.ddct(ary, inversion: true) -> [*Float]
 * 
 * Perform FFT with the first argument +ary+ as a numerical sequence.<br>
 * If the keyword argument +inversion+ is +true+, perform an inverse FFT.
 * 
 * [Function type]            Discrete Cosine Transform
 * [Definition]
 *   case1:: IDCT (excluding scale)
 *     :: <tt>C[k] = sum_j=0^n-1 a[j]*cos(pi*j*(k+1/2)/n), 0<=k<n</tt>
 *   case2:: DCT
 *     :: <tt>C[k] = sum_j=0^n-1 a[j]*cos(pi*(j+1/2)*k/n), 0<=k<n</tt>
 * [+ary+ sequence type]      All +Float+
 * [Array size requirement]   n >= 2, n = power of 2
 * 
 *   def sinewave(amp, f0, fs, n)
 *     amp * Math.sin(2 * Math::PI * f0 * n / fs)
 *   end
 *   
 *   OouraFFT.ddct(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
 *   #=> [0.6284174365157309,
 *   #=> -0.6284174365157309,
 *   #=>  0.18707572033318604,
 *   #=> -0.18707572033318612,
 *   #=>  0.08352232973991239,
 *   #=> -0.08352232973991236,
 *   #=>  0.02486404592245728,
 *   #=> -0.024864045922457167]
 */
static VALUE
fft_ddct(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_ddct_inline);
}

VALUE
rb_oourafft_ddct(VALUE ary, int inversion)
{
	return api_inline(ary, inversion, fft_ddct_inline);
}

/******************************************************************************/

#include "internal/solver/ooura_fft/ddst.h" // fft_ddst_inline()
/*
 * Document-method: ddst
 * 
 * call-seq:
 *   OouraFFT.ddst(ary)                  -> [*Float]
 *   OouraFFT.ddst(ary, inversion: true) -> [*Float]
 * 
 * Perform FFT with the first argument +ary+ as a numerical sequence.<br>
 * If the keyword argument +inversion+ is +true+, perform an inverse FFT.
 * 
 * [Function type]            Discrete Sine Transform
 * [Definition]
 *   case1:: IDST (excluding scale)
 *     :: <tt>S[k] = sum_j=1^n A[j]*sin(pi*j*(k+1/2)/n), 0<=k<n</tt>
 *   case2:: DST
 *     :: <tt>S[k] = sum_j=0^n-1 a[j]*sin(pi*(j+1/2)*k/n), 0<k<=n</tt>
 * [+ary+ sequence type]      All +Float+
 * [Array size requirement]   n >= 2, n = power of 2
 * 
 *   def sinewave(amp, f0, fs, n)
 *     amp * Math.sin(2 * Math::PI * f0 * n / fs)
 *   end
 *   
 *   OouraFFT.ddst(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
 *   #=> [0.875,
 *   #=>  0.12500000000000003,
 *   #=> -0.12499999999999997,
 *   #=>  0.125,
 *   #=> -0.125,
 *   #=>  0.12500000000000006,
 *   #=> -0.12499999999999994,
 *   #=>  0.12499999999999994]

 */
static VALUE
fft_ddst(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_ddst_inline);
}

VALUE
rb_oourafft_ddst(VALUE ary, int inversion)
{
	return api_inline(ary, inversion, fft_ddst_inline);
}

/******************************************************************************/

#include "internal/solver/ooura_fft/dfct.h" // fft_dfct_inline()
/*
 * Document-method: dfct
 * 
 * call-seq:
 *   OouraFFT.dfct(ary)                  -> [*Float]
 *   OouraFFT.dfct(ary, inversion: true) -> [*Float]
 * 
 * Perform FFT with the first argument +ary+ as a numerical sequence.<br>
 * If the keyword argument +inversion+ is +true+, perform an inverse FFT.
 * 
 * [Function type]            Cosine Transform of RDFT (Real Symmetric DFT)
 * [Definition]               <tt>C[k] = sum_j=0^n a[j]*cos(pi*j*k/n), 0<=k<=n</tt>
 * [+ary+ sequence type]      All +Float+
 * [Array size requirement]   n >= 2, n = power of 2
 * 
 *   def sinewave(amp, f0, fs, n)
 *     amp * Math.sin(2 * Math::PI * f0 * n / fs)
 *   end
 *   
 *   OouraFFT.dfct(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
 *   #=> [1.1441462984511075,
 *   #=> -0.30353826116690874,
 *   #=> -0.21410561232180816,
 *   #=>  0.08422719461241164,
 *   #=> -0.14986404592245728,
 *   #=>  0.10711452157013322,
 *   #=> -0.13944777827146557,
 *   #=>  0.11219654498436385]
 */
static VALUE
fft_dfct(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_dfct_inline);
}

VALUE
rb_oourafft_dfct(VALUE ary, int inversion)
{
	return api_inline(ary, inversion, fft_dfct_inline);
}

/******************************************************************************/

#include "internal/solver/ooura_fft/dfst.h" // fft_dfst_inline()
/*
 * Document-method: dfst
 * 
 * call-seq:
 *   OouraFFT.dfst(ary)                  -> [*Float]
 *   OouraFFT.dfst(ary, inversion: true) -> [*Float]
 * 
 * Perform FFT with the first argument +ary+ as a numerical sequence.<br>
 * If the keyword argument +inversion+ is +true+, perform an inverse FFT.
 * 
 * [Function type]            Sine Transform of RDFT (Real Anti-symmetric DFT)
 * [Definition]               <tt>S[k] = sum_j=1^n-1 a[j]*sin(pi*j*k/n), 0<k<n</tt>
 * [+ary+ sequence type]      All +Float+
 * [Array size requirement]   n >= 2, n = power of 2
 * 
 *   def sinewave(amp, f0, fs, n)
 *     amp * Math.sin(2 * Math::PI * f0 * n / fs)
 *   end
 *   
 *   OouraFFT.dfst(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
 *   #=> [0.0,
 *   #=>  0.8406080372841989,
 *   #=> -0.3229641637954819,
 *   #=>  0.19308574608608534,
 *   #=> -0.12744889477603982,
 *   #=>  0.08469937042371564,
 *   #=> -0.05236611372238342,
 *   #=>  0.025114880435281595]
 */
static VALUE
fft_dfst(int argc, VALUE *argv, VALUE unused_obj)
{
	return fft_callback(argc, argv, fft_dfst_inline);

}

VALUE
rb_oourafft_dfst(VALUE ary, int inversion)
{
	return api_inline(ary, inversion, fft_dfst_inline);
}

/******************************************************************************/

static void 
InitVM_FFTMain(void)
{
	rb_define_module_function(rb_mOouraFFT, "cdft", fft_cdft, -1);
	rb_define_module_function(rb_mOouraFFT, "rdft", fft_rdft, -1);
	rb_define_module_function(rb_mOouraFFT, "ddct", fft_ddct, -1);
	rb_define_module_function(rb_mOouraFFT, "ddst", fft_ddst, -1);
	rb_define_module_function(rb_mOouraFFT, "dfct", fft_dfct, -1);
	rb_define_module_function(rb_mOouraFFT, "dfst", fft_dfst, -1);
	/*  The using thread model name. Supports in Windows and POSIX. */
	rb_define_const(rb_mOouraFFT, "USING_THREAD", rb_str_new_cstr((const char *)USING_THREAD));
}
