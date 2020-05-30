#include "arch/system.h"
#include "arch/uart.h"
#include "printf/printf.h"
#include "std_lib/atomic.h"

static volatile uint64_t init = 0;
static volatile uint64_t lock = 0;

void kernel_main();

void system_init() {
	uart_init();

	printf("System init entered\n");

	// Signal the other cores
	init = 1;
}

void kernel_main() {
	unsigned int coreId = get_core_id();

	printf("Hello from core %u\n", coreId);

	if (coreId == 0) {
		while (1) {
			char c = uart_recv();
			// Serial sends DEL when hitting backspace
			// So we have to manually fix it up
			// Also, \b just moves the cursor back. It doesn't actually remove the character
			if (c == 127 || c == '\b') {
				uart_send('\b');
				uart_send(' ');
				uart_send('\b');
			} else {
				uart_send(c);
			}
		}
	} else {
		while (1) {
			asm volatile("nop");
		}
	}
}