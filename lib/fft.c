/*******************************************************************************
	dsp_fft.c -- FFT
*******************************************************************************/
#include <ruby.h>
#include "abi.h"

// 初期設定: エントリポイント
static void InitVM_FFTMain(void);
void
InitVM_FFT(void)
{
	InitVM(FFTMain);
}

static VALUE
fft_cdft_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long sz = RARRAY_LEN(ary);
	int *ip;
	double *a, *w;
	
	if ((INT_MAX / 2) < sz)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (sz < 1)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 1, was %ld)", sz);
	else if (!ispow2l(sz))
		rb_raise(rb_eRangeError, "size must be 2^m");
	
	a = ALLOC_N(double, sz*2);
	ip = ALLOC_N(int, (int)(2+sqrt(sz)));
	w = ALLOC_N(double, sz/2);
	
	for (volatile long i = 0; i < sz; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		if (TYPE(elem) != T_COMPLEX)
			elem = rb_Complex(elem);
		const double real = NUM2DBL(rb_complex_real(elem));
		const double imag = NUM2DBL(rb_complex_imag(elem));
		a[2*i] = real;
		a[2*i+1] = imag;
	}
	
	ip[0] = 0;
	
	cdft(2*sz, invertible, a, ip, w);
	
	retval = rb_ary_new2(sz);
	
	for (volatile long i = 0; i < sz; i++)
	{
		rb_ary_store(retval, i, rb_dbl_complex_new(a[2*i], a[2*i+1]));
	}
	
	xfree(a);
	xfree(ip);
	xfree(w);
	
	return retval;
}

/*
 *  call-seq:
 *    FFT.cdft(ary) -> [*Complex]
 *    FFT.cdft(ary, inverse: true) -> [*Complex]
 *  
 *  引数aryを複素数列とみなし，離散フーリエ変換を行う．
 *  
 *  @ary     ... 配列クラスを指定する．要素は全てComplexクラスであるとみなされる．
 *               そうでない要素があった場合，暗黙の型変換を行う．
 *  @inverse ... キーワード引数．ブーリアンである．デフォルトはfalse．
 *               trueにすると逆FFTにスイッチする．
 *  retval   ... 複素数列のArray
 */
static VALUE
fft_cdft(int argc, VALUE *argv, VALUE unused_obj)
{
	VALUE ary, inverse;
	int invertible = 1;
	rb_scan_args(argc, argv, "11", &ary, &inverse);
	
	if (TYPE(inverse) == T_HASH && RTEST(rb_hash_lookup2(inverse, MAKE_SYM("inverse"), Qnil)))
		invertible = -1;
	
	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eArgError, "not an Array");

	return fft_cdft_inline(ary, invertible);
}


static VALUE
fft_rdft_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long sz = RARRAY_LEN(ary);
	int *ip;
	double *a, *w;
	
	if (INT_MAX < sz)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (sz < 2)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 2, was %ld)", sz);
	else if (!ispow2l(sz))
		rb_raise(rb_eRangeError, "size must be 2^m");
	
	a = ALLOC_N(double, sz);
	ip = ALLOC_N(int, (int)(2+sqrt(sz/2)));
	w = ALLOC_N(double, sz/2);
	
	for (volatile long i = 0; i < sz; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		a[i] = NUM2DBL(elem);
	}
	
	ip[0] = 0;
	
	rdft(sz, invertible, a, ip, w);
	
	retval = rb_ary_new2(sz);
	
	for (volatile long i = 0; i < sz; i++)
	{
		rb_ary_store(retval, i, DBL2NUM(a[i]));
	}
	
	xfree(a);
	xfree(ip);
	xfree(w);
	
	return retval;
}

/*
 *  call-seq:
 *    FFT.rdft(ary) -> [*Float]
 *    FFT.rdft(ary, inverse: true) -> [*Float]
 *  
 *  引数aryを実数数列とみなし，離散フーリエ変換を行う．
 *  
 *  @ary     ... 配列クラスを指定する．要素は全てFloatクラスであるとみなされる．
 *               そうでない要素があった場合，暗黙の型変換を行う．
 *  @inverse ... キーワード引数．ブーリアンである．デフォルトはfalse．
 *               trueにすると逆FFTにスイッチする．
 *  retval   ... 実数数列のArray
 */
static VALUE
fft_rdft(int argc, VALUE *argv, VALUE unused_obj)
{
	VALUE ary, inverse;
	int invertible = 1;
	rb_scan_args(argc, argv, "11", &ary, &inverse);
	
	if (TYPE(inverse) == T_HASH && RTEST(rb_hash_lookup2(inverse, MAKE_SYM("inverse"), Qnil)))
		invertible = -1;
	
	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eArgError, "not an Array");

	return fft_rdft_inline(ary, invertible);
}

static VALUE
fft_ddct_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long sz = RARRAY_LEN(ary);
	int *ip;
	double *a, *w;
	
	if (INT_MAX < sz)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (sz < 2)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 2, was %ld)", sz);
	else if (!ispow2l(sz))
		rb_raise(rb_eRangeError, "size must be 2^m");
	
	a = ALLOC_N(double, sz);
	ip = ALLOC_N(int, (int)(2+sqrt(sz/2)));
	w = ALLOC_N(double, sz*5/4);
	
	for (volatile long i = 0; i < sz; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		a[i] = NUM2DBL(elem);
	}
	
	ip[0] = 0;
	
	ddct(sz, invertible, a, ip, w);
	
	retval = rb_ary_new2(sz);
	
	for (volatile long i = 0; i < sz; i++)
	{
		rb_ary_store(retval, i, DBL2NUM(a[i]));
	}
	
	xfree(a);
	xfree(ip);
	xfree(w);
	
	return retval;
}

/*
 *  call-seq:
 *    FFT.ddct(ary) -> [*Float]
 *    FFT.ddct(ary, inverse: true) -> [*Float]
 *  
 *  引数aryを実数数列とみなし，離散余弦変換を行う．
 *  
 *  @ary     ... 配列クラスを指定する．要素は全てFloatクラスであるとみなされる．
 *               そうでない要素があった場合，暗黙の型変換を行う．
 *  @inverse ... キーワード引数．ブーリアンである．デフォルトはfalse．
 *               trueにすると逆FFTにスイッチする．
 *  retval   ... 実数数列のArray
 */
static VALUE
fft_ddct(int argc, VALUE *argv, VALUE unused_obj)
{
	VALUE ary, inverse;
	int invertible = 1;
	rb_scan_args(argc, argv, "11", &ary, &inverse);
	
	if (TYPE(inverse) == T_HASH && RTEST(rb_hash_lookup2(inverse, MAKE_SYM("inverse"), Qnil)))
		invertible = -1;
	
	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eArgError, "not an Array");

	return fft_ddct_inline(ary, invertible);
}

static VALUE
fft_ddst_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long sz = RARRAY_LEN(ary);
	int *ip;
	double *a, *w;
	
	if (INT_MAX < sz)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (sz < 2)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 2, was %ld)", sz);
	else if (!ispow2l(sz))
		rb_raise(rb_eRangeError, "size must be 2^m");
	
	a = ALLOC_N(double, sz);
	ip = ALLOC_N(int, (int)(2+sqrt(sz/2)));
	w = ALLOC_N(double, sz*5/4);
	
	for (volatile long i = 0; i < sz; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		a[i] = NUM2DBL(elem);
	}
	
	ip[0] = 0;
	
	ddst(sz, invertible, a, ip, w);
	
	retval = rb_ary_new2(sz);
	
	for (volatile long i = 0; i < sz; i++)
	{
		rb_ary_store(retval, i, DBL2NUM(a[i]));
	}
	
	xfree(a);
	xfree(ip);
	xfree(w);
	
	return retval;
}

/*
 *  call-seq:
 *    FFT.ddst(ary) -> [*Float]
 *    FFT.ddst(ary, inverse: true) -> [*Float]
 *  
 *  引数aryを実数数列とみなし，離散正弦変換を行う．
 *  
 *  @ary     ... 配列クラスを指定する．要素は全てFloatクラスであるとみなされる．
 *               そうでない要素があった場合，暗黙の型変換を行う．
 *  @inverse ... キーワード引数．ブーリアンである．デフォルトはfalse．
 *               trueにすると逆FFTにスイッチする．
 *  retval   ... 実数数列のArray
 */
static VALUE
fft_ddst(int argc, VALUE *argv, VALUE unused_obj)
{
	VALUE ary, inverse;
	int invertible = 1;
	rb_scan_args(argc, argv, "11", &ary, &inverse);
	
	if (TYPE(inverse) == T_HASH && RTEST(rb_hash_lookup2(inverse, MAKE_SYM("inverse"), Qnil)))
		invertible = -1;
	
	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eArgError, "not an Array");

	return fft_ddst_inline(ary, invertible);
}


static VALUE
fft_dfct_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long sz = RARRAY_LEN(ary), n = sz - 1;
	int *ip;
	double *a, *w, *t;
	
	if (INT_MAX < n)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (n < 2)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 2, was %ld)", sz);
	else if (!ispow2l(n))
		rb_raise(rb_eRangeError, "size must be 2^m+1");
	
	a = ALLOC_N(double, sz);
	ip = ALLOC_N(int, (int)(2+sqrt(n/4)));
	t = ALLOC_N(double, n/2+1);
	w = ALLOC_N(double, sz*5/8);
	
	for (volatile long i = 0; i < sz; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		if (invertible == -1)
			a[i] = NUM2DBL(elem) * 0.5;
		else
			a[i] = NUM2DBL(elem);
	}
	
	ip[0] = 0;
	
	dfct(n, a, t, ip, w);
	
	retval = rb_ary_new2(sz);
	
	for (volatile long i = 0; i < sz; i++)
	{
		if (invertible == -1)
			rb_ary_store(retval, i, DBL2NUM(a[i] * 2.0 / n));
		else
			rb_ary_store(retval, i, DBL2NUM(a[i]));
	}
	
	xfree(a);
	xfree(ip);
	xfree(t);
	xfree(w);
	
	return retval;
}

/*
 *  call-seq:
 *    FFT.dfct(ary) -> [*Float]
 *    FFT.dfct(ary, inverse: true) -> [*Float]
 *  
 *  引数aryを実数数列とみなし，実数離散フーリエ変換の余弦変換(実数対称離散フーリエ変換)を行う．
 *  
 *  @ary     ... 配列クラスを指定する．要素は全てFloatクラスであるとみなされる．
 *               そうでない要素があった場合，暗黙の型変換を行う．
 *  @inverse ... キーワード引数．ブーリアンである．デフォルトはfalse．
 *               trueにすると逆FFTにスイッチする．
 *  retval   ... 実数数列のArray
 */
static VALUE
fft_dfct(int argc, VALUE *argv, VALUE unused_obj)
{
	VALUE ary, inverse;
	int invertible = 1;
	rb_scan_args(argc, argv, "11", &ary, &inverse);
	
	if (TYPE(inverse) == T_HASH && RTEST(rb_hash_lookup2(inverse, MAKE_SYM("inverse"), Qnil)))
		invertible = -1;
	
	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eArgError, "not an Array");

	return fft_dfct_inline(ary, invertible);
}


static VALUE
fft_dfst_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long sz = RARRAY_LEN(ary), n = sz + 1;
	int *ip;
	double *a, *w, *t;
	
	if (INT_MAX < n)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (n < 2)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 2, was %ld)", sz);
	else if (!ispow2l(n))
		rb_raise(rb_eRangeError, "size must be 2^m-1");
	
	a = ALLOC_N(double, sz);
	ip = ALLOC_N(int, (int)(2+sqrt(n/4)));
	t = ALLOC_N(double, n/2);
	w = ALLOC_N(double, sz*5/8);
	
	for (volatile long i = 0; i < sz; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		a[i] = NUM2DBL(elem);
	}
	
	ip[0] = 0;
	
	dfst(n, a, t, ip, w);
	
	retval = rb_ary_new2(sz);
	
	for (volatile long i = 0; i < sz; i++)
	{
		if (invertible == -1)
			rb_ary_store(retval, i, DBL2NUM(a[i] * 2.0 / n));
		else
			rb_ary_store(retval, i, DBL2NUM(a[i]));
	}
	
	xfree(a);
	xfree(ip);
	xfree(t);
	xfree(w);
	
	return retval;
}

/*
 *  call-seq:
 *    FFT.dfst(ary) -> [*Float]
 *    FFT.dfst(ary, inverse: true) -> [*Float]
 *  
 *  引数aryを実数数列とみなし，実数離散フーリエ変換の正弦変換(実数非対称離散フーリエ変換)を行う．
 *  
 *  @ary     ... 配列クラスを指定する．要素は全てFloatクラスであるとみなされる．
 *               そうでない要素があった場合，暗黙の型変換を行う．
 *  @inverse ... キーワード引数．ブーリアンである．デフォルトはfalse．
 *               trueにすると逆FFTにスイッチする．
 *  retval   ... 実数数列のArray
 */
static VALUE
fft_dfst(int argc, VALUE *argv, VALUE unused_obj)
{
	VALUE ary, inverse;
	int invertible = 1;
	rb_scan_args(argc, argv, "11", &ary, &inverse);
	
	if (TYPE(inverse) == T_HASH && RTEST(rb_hash_lookup2(inverse, MAKE_SYM("inverse"), Qnil)))
		invertible = -1;
	
	if (TYPE(ary) != T_ARRAY)
		rb_raise(rb_eArgError, "not an Array");

	return fft_dfst_inline(ary, invertible);
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
