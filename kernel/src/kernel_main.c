#include "arch/system.h"
#include "arch/uart.h"
#include "printf/printf.h"
#include "std_lib/atomic.h"

void kernel_main();

void system_init() {
	uart_init();

	printf("System init entered\n");

	unsigned currentEL = get_el();
	printf("Current EL: %d\n", currentEL);
}

void kernel_main() {
	unsigned int coreId = get_core_id();

	printf("Hello from core %u\n", coreId);

	if (coreId == 0) {
		while (1) {
			char c = uart_recv();
			// If user presses ctrl+r, trigger a reboot
			if (c == 18) {
				reboot();
			}

			printf("Received: %d\n", c);
		}
	} else {
		while (1) {
			asm volatile("nop");
		}
	}
}
