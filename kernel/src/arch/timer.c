#include "arch/timer.h"

#include "arch/exceptions.h"
#include "arch/interupts.h"
#include "arch/mmio.h"

#include "printf/printf.h"

void wait_cycles(unsigned int n) {
	for (unsigned int i = 0; i < n; ++i) {
		asm volatile("nop");
	}
}

#define SYS_TIMER_FREQ (1000000) // 1 MHz. Taken from linux device tree
uint32_t s_timer3Interval;

void timer3_interupt_handler(void *param) {
	(void)param;

	printf("Timer 3 fired\n");

	// Reset the compare to the next offset
	uint32_t compare = mmio_load(SYS_TIMER_C3);
	compare += s_timer3Interval;
	mmio_store(SYS_TIMER_C3, compare);

	// Confirm the IRQ to clear the line
	mmio_store(SYS_TIMER_CS, SYS_TIMER_CS_M3);
}

void sys_timer3_init() {
	connect_irq(ARM_IRQ_SPI_SYS_TIMER3, timer3_interupt_handler, NULL);
}

void sys_timer3_term() {
	disconnect_irq(ARM_IRQ_SPI_SYS_TIMER3);
}

void sys_timer3_set(uint32_t interval_ms) {
	// Calculate the timer interval from the millisecond interval
	uint32_t interval = interval_ms * SYS_TIMER_FREQ / 1000;

	// Set the compare to the next value
	s_timer3Interval = interval;
	uint32_t current = mmio_load(SYS_TIMER_CLO);
	current += interval;
	mmio_store(SYS_TIMER_C3, current);
}
