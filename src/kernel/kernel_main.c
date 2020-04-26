#include "arch/system.h"
#include "arch/timer.h"
#include "arch/uart.h"
#include "std_lib/atomic.h"

#include "printf.h"

static volatile uint64_t init = 0;
static volatile uint64_t lock = 0;

void kernel_main();

void system_init() {
	uart_init();

	printf("Current EL: %u\n", get_el());

	// Signal the other cores
	atomic_exchange_mb_u64(&init, 1);

	kernel_main();
}

void core_wait_for_instructions() {
	// Wait for the system to be initialized
	while (atomic_compare_exchange_mb_u64(&init, 1, 1) != 1) {
		asm volatile("nop");
	}

	kernel_main();
}

void kernel_main() {
	unsigned int coreId = get_core_id();

	// Lock access to the terminal, so we don't get jumbled, interleaved words
	while (atomic_compare_exchange_mb_u64(&lock, 0, 1) != 0) {
		asm volatile("nop");
	}
	printf("Hello from core %u\n", coreId);
	// Unlock
	atomic_exchange_mb_u64(&lock, 0);

	if (coreId == 0) {
		while (1) {
			uart_send(uart_recv());
		}
	} else {
		while (1) {
			asm volatile("nop");
		}
	}
}
