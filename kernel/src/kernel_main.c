#include "arch/exceptions.h"
#include "arch/gic-400.h"
#include "arch/system.h"
#include "arch/timer.h"
#include "arch/uart.h"

#include "std_lib/atomic.h"

#include "printf/printf.h"

void kernel_main();

void system_init() {
	uart_init();

	printf("System init entered\n");

	unsigned currentEL = get_el();
	printf("Current EL: %d\n", currentEL);

	exception_handler_init();
	gic_400_init();
	printf("Initialized exception handler\n");

	enable_irq();
	printf("IRQ enabled\n");
}

void kernel_main() {
	unsigned int coreId = get_core_id();

	sys_timer3_set(1000);
	sys_timer3_init();
	printf("System timer 3 initialized\n");

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
