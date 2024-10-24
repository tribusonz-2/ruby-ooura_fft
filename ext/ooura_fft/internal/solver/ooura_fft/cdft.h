#ifndef INTERNAL_SOLVER_FFT_CDFT_H
#define INTERNAL_SOLVER_FFT_CDFT_H

static inline VALUE
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
			elem = rb_Complex1(elem);
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

#endif /* INTERNAL_SOLVER_FFT_CDFT */
