#ifndef INTERNAL_SOLVER_FFT_DFCT_H
#define INTERNAL_SOLVER_FFT_DFCT_H

static inline VALUE
fft_dfct_inline(VALUE ary, int invertible)
{
	VALUE retval;
	long n = RARRAY_LEN(ary), sz = n + 1;
	int *ip;
	double *a, *w, *t;
	
	if (INT_MAX < n)
		rb_raise(rb_eRangeError, "biggest array size");
	else if (n < 2)
		rb_raise(rb_eRangeError, "unavailable array size (n >= 2, was %ld)", n);
	else if (!ispow2l(n))
		rb_raise(rb_eRangeError, "size must be 2^m");
	
	a = ALLOC_N(double, sz);
	ip = ALLOC_N(int, (int)(2+sqrt(n/4)));
	t = ALLOC_N(double, sz/2);
	w = ALLOC_N(double, n*5/8);
	
	for (volatile long i = 0; i < n; i++)
	{
		VALUE elem = rb_ary_entry(ary, i);
		if (invertible == -1)
			a[i] = NUM2DBL(elem) * 0.5;
		else
			a[i] = NUM2DBL(elem);
	}
	
	ip[0] = 0;
	
	dfct(n, a, t, ip, w);
	
	retval = rb_ary_new2(n);
	
	for (volatile long i = 0; i < n; i++)
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

#endif /* INTERNAL_SOLVER_FFT_DFCT */
