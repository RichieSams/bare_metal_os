#include "arch/timer.h"

void wait_cycles(unsigned int n) {
	for (unsigned int i = 0; i < n; ++i) {
		asm volatile("nop");
	}
}
