/*******************************************************************************
	ooura_fft.c - EntryPoint
	
	$author$
*******************************************************************************/
#include <ruby.h>
#define  USE_GLOBAL_VARIABLE
#include "ooura_fft/globals.h"
#include "missing/missing.h"
#include "internal/setting/ooura_fft.h"
#include "abi/fftsg/fftsg.h"

#ifndef HAVE_ISPOW2L
# include "missing/ispow2l.c"
#endif

// ABI of OouraFFT
#include "abi/fftsg/fftsg.c"

#define RDOC_PARSE_DUMMY  0

void
Init_ooura_fft(void)
{
	/* 
	 * Document-class: OouraFFT
	 * 
	 * We provide Professor Ooura's FFT as a front end.
	 * 
	 * There are six FFT routines available, each with the following:
	 *   cdft: Complex Discrete Fourier Transform
	 *   rdft: Real Discrete Fourier Transform
	 *   ddct: Discrete Cosine Transform
	 *   ddst: Discrete Sine Transform
	 *   dfct: Cosine Transform of RDFT (Real Symmetric DFT)
	 *   dfst: Sine Transform of RDFT (Real Anti-symmetric DFT)
	 * 
	 * 
	 * Copyright:
	 * :: (C) 1996-2001 Takuya OOURA
	 */
	rb_mOouraFFT = rb_define_module("OouraFFT");
	
#if RDOC_PARSE_DUMMY
	rb_define_module_function(rb_mOouraFFT, "cdft", fft_cdft, -1); // in fft.c
	rb_define_module_function(rb_mOouraFFT, "rdft", fft_rdft, -1); // in fft.c
	rb_define_module_function(rb_mOouraFFT, "ddct", fft_ddct, -1); // in fft.c
	rb_define_module_function(rb_mOouraFFT, "ddst", fft_ddst, -1); // in fft.c
	rb_define_module_function(rb_mOouraFFT, "dfct", fft_dfct, -1); // in fft.c
	rb_define_module_function(rb_mOouraFFT, "dfst", fft_dfst, -1); // in fft.c
	/*  The using thread model name. Supports in Windows and POSIX. */
	rb_define_const(rb_mOouraFFT, "USING_THREAD", rb_str_new_cstr((const char *)USING_THREAD));
#else
	InitVM(FFT);
#endif
}
