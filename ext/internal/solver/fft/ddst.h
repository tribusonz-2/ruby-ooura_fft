#ifndef INTERNAL_SOLVER_FFT_DDST_H
#define INTERNAL_SOLVER_FFT_DDST_H

static inline VALUE
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

#endif /* INTERNAL_SOLVER_FFT_DDST */
