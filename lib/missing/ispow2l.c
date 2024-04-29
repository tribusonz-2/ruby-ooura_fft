#include <stdbool.h>

bool
ispow2l(register long n)
{
	if (n <= 0) return false;
	for (; (n & 1) == 0; n >>= 1);
	return n == 1;
}
