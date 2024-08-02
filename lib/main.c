/*******************************************************************************
	main.c -
	
	$author$
*******************************************************************************/
#include <ruby.h>
#define  USE_GLOBAL_VARIABLE
#include "ruby/fft/globals.h"

void InitVM_FFT(void);

void
Init_fft(void)
{
	rb_mFFT = rb_define_module("FFT");
	
	InitVM(FFT);
}
