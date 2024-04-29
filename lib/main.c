/*******************************************************************************
	main.c -- Entry Point (Initializer)
*******************************************************************************/
#include <ruby.h>
#define  USE_GLOBAL_VARIABLE
#include "abi.h"

void
Init_fft(void)
{
	rb_mFFT = rb_define_module("FFT");
	
	InitVM(FFT);
}
